#include "main.h"
#define ALIVEMSGFREQSECS 2
#define MENUHOLD_MS 1500
#define DEBOUNCE 120

#define RETURN_GPIO 9
#define SELECT_GPIO 8
#define NEXT_GPIO   7
struct AlivePacket{
  std::string message;
  volatile bool outstandingPrint; 
  AlivePacket(){
    message = std::string();
    outstandingPrint = false;
  }
} alivePacket;


int64_t alarm_callback(alarm_id_t id, void *user_data) {
  typedef  volatile bool* DataFormat;
  DataFormat ptr = (DataFormat)user_data;
  if(*ptr && (digitalRead(RETURN_GPIO)==PinStatus::HIGH)){
    timer_fired = true;
  };
  holdCounting = false;
  holding = false;

  return 0;
};

int logFunctionResult(const std::string functionMessage, int (*function_ptr)(), bool logToConsole = true){
  if(logToConsole){
  std::string scopeMsg = std::string("[SETUP][");
  scopeMsg.append(functionMessage);
  scopeMsg.append("] ");
  std::string logMessage = scopeMsg;
  logMessage.append("Starting...");
  Serial.println(logMessage.c_str());
  int result = function_ptr();
  logMessage = scopeMsg;
  if(result == 0){
    logMessage.append("OK!");
  } else {
    logMessage.append("ERROR! %i", result);
  }
  Serial.println(logMessage.c_str());
  return result;
  } else {
    return function_ptr();
  }
};


int pinSetup(){
  //RGB led
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);
  //encoder
  pinMode(12, INPUT_PULLUP); //CLICK
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  //button
  pinMode(9, INPUT_PULLDOWN); //CLICK
  pinMode(8, INPUT_PULLDOWN);
  pinMode(7, INPUT_PULLDOWN);
  //buzzer
  pinMode(6, OUTPUT);
  return 0;
};

int createUISupervisor(){
  
  return 0;
};

int createUIdisplay(){
  screen = LCUIDisplay();
  return 0;
};

int addDisplayToSupervisor(){
  uiSupervisor.set_UIDisplay(&screen);
  return 0;
};

int finalizeSupervisor(){
  uiSupervisor.finalize();
  return 0;
};

int startupSupervisor(){
  uiSupervisor.startup();
  return 0;
};

void GPIOInterruptHandler_SINGULAR(){
  if(holding){
    if(digitalRead(RETURN_GPIO) == LOW){
      holding = false;
      button_pressed = true;
      GPIO = RETURN_GPIO;
  };
  }else if(!time_reached(DeBounce)){
  return;
 }else if(digitalRead(RETURN_GPIO) == HIGH){
    if(!holdCounting){
      build_alarm = true;
    };
  } else if (digitalRead(SELECT_GPIO) == HIGH){
  button_pressed = true;
  GPIO = SELECT_GPIO;
  //uiSupervisor._trigger_select();
  }else if (digitalRead(NEXT_GPIO) == HIGH){
    button_pressed = true;
    GPIO = NEXT_GPIO;
  }else {
    button_pressed = false;
  };
};


int attachSupervisorInterruptSingular(){
  attachInterrupt(digitalPinToInterrupt(9), GPIOInterruptHandler_SINGULAR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), GPIOInterruptHandler_SINGULAR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), GPIOInterruptHandler_SINGULAR, CHANGE);
  return 0;
};

int setupInitialAlarmPool(){
  alarm_pool_primary = alarm_pool_create(2,16);
  alarm_pool_secondary = alarm_pool_create(3,32);
  return 0;
}



bool repeatingPrintAliveFunct(repeating_timer* rt){
  AlivePacket* dataPtr = (AlivePacket*)(*rt).user_data;
  if((*dataPtr).outstandingPrint == false){
    (*dataPtr).message.clear();
    (*dataPtr).message.append("Runtime[seconds]: ");
    (*dataPtr).message.append(std::to_string((int)(time_us_64()/(1000*1000))).c_str());
    (*dataPtr).outstandingPrint = true;
  } 
  return true;
};



int setupMainMenuOnHold(){
  //returnHomeTimestamp = get_absolute_time();
  return 0;
}

int setupAlivePrintToSerial(){
  if(createTimeout(alarm_pool_primary, ALIVEMSGFREQSECS*1000000, repeatingPrintAliveFunct,(void*)&alivePacket, (repeating_timer_t*)&rtInst)){
    return 0;
  } else {
    return 1;
  }
};


void setupHoldAlarm(){
  build_alarm = false;
  holding = true;
  holdCounting = true;
  homeButtonAlarmID = createTimeout_single(alarm_pool_secondary, 1000, alarm_callback, (void*)&holding, true);
};

void cancelHoldAlarm(){
  remove_alarm = false;
  holdCounting = false;
   alarm_pool_cancel_alarm(alarm_pool_secondary, homeButtonAlarmID);
   homeButtonAlarmID = 0;
};





void Supervisor::GPIOInterruptHandler_RETURN(){
  button_pressed = true;
  uiSupervisor._trigger_return();
}

void Supervisor::GPIOInterruptHandler_SELECT(){
  button_pressed = true;
  uiSupervisor._trigger_select();
}

void Supervisor::GPIOInterruptHandler_NEXT(){
  button_pressed = true;
  uiSupervisor._trigger_next();
}

void setup() {
  USBSetup();
  GPIO = -1;
  printFreq = get_absolute_time();
  alivePacket = AlivePacket();
  holding = false;
  holdCounting = false;
  callbackResult = false;
  timer_fired = false;
  button_pressed = false;
  build_alarm = false;
  remove_alarm = false;
  DeBounce = make_timeout_time_ms(DEBOUNCE);
  homeButtonAlarmID = 0;
  alivePacket.outstandingPrint = true;
  alivePacket.message.reserve(64); //without this timer breaks if string has to be resized in the callback

  logFunctionResult("Pin Setup", pinSetup);

  //logFunctionResult("I2C 1602 LCD", ScreenSetup);
  //logFunctionResult("UISupervisor init", createUISupervisor);
  logFunctionResult("UIDisplay instantiate", createUIdisplay);
  logFunctionResult("Add display to supervisor", addDisplayToSupervisor);
  logFunctionResult("Finalize Supervisor", finalizeSupervisor);
  logFunctionResult("Initial Alarm Pool setup", setupInitialAlarmPool);
  //assignFunctionsToButtons();
  
  //createTimeout(sample, 25000,callbackFunct,(void*)pinButtonDict, (repeating_timer_t*)&rtInst);
  //rotary = RotaryEncoder();
  //setupUISupervisor();
  //Serial.println(std::to_string(uiSupervisor.debugFunc()).c_str());
  //timeStamp = make_timeout_time_ms(100);
  
  //setupInterrupts();
  //Serial.println(std::to_string(timer.getTimeLeftAsSeconds()).c_str());
  //logFunctionResult("Supervisor STARTUP", startupSupervisor);
  logFunctionResult("Repeating ALIVE timer activation", setupAlivePrintToSerial);
  logFunctionResult("Attach Supervisor interrupt", attachSupervisorInterruptSingular);
  
  uiSupervisor.startup();
};



void loop() {
  
  while(!time_reached(printFreq) && !button_pressed && !(GPIO>0) && !timer_fired){};
  if(timer_fired){
    timer_fired = false;
    Serial.println("LONG PRESS!");
    uiSupervisor.HOME.trigger_function();
    holding = false;
  };
  if(time_reached(printFreq)){
    //Serial.println("test");
    printFreq = make_timeout_time_ms(1000);
  };

  if(alivePacket.outstandingPrint){
    alivePacket.outstandingPrint = false;
    Serial.println(alivePacket.message.c_str());
  };

  

  if(build_alarm){
      setupHoldAlarm();
  }else if(GPIO>0){
    Serial.println("button_pressed");
    //button_pressed = false;
    switch (GPIO)
    {
    case RETURN_GPIO:
      uiSupervisor.REQUIRED_BUTTONS.RETURN.trigger_function();
      break;
    case SELECT_GPIO:
      uiSupervisor.REQUIRED_BUTTONS.SELECT.trigger_function();
      break;
    case NEXT_GPIO:
    uiSupervisor.REQUIRED_BUTTONS.NEXT.trigger_function();
      break;
    default:
    GPIO = -1;
      break;
    };
    GPIO = 0;
  }else if(remove_alarm){cancelHoldAlarm();};
  DeBounce = make_timeout_time_ms(50);
  button_pressed = false;
  uiSupervisor.run();
  };

  

  
    
    