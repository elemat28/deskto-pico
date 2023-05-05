#include "main.h"
#define ALIVEMSGFREQSECS 2
#define MENUHOLD_MS 1500
#define DEBOUNCE 120

#define RETURN_GPIO 9
#define SELECT_GPIO 8
#define NEXT_GPIO   7

volatile bool GPIO_STATE[32] = {false};
const std::vector<int> DEFINED_BUTTONS = {RETURN_GPIO, SELECT_GPIO, NEXT_GPIO};

struct PinStruct {
  const int gpio;
  volatile bool state;
  PinStruct(int GPIO): gpio(GPIO){
    state = false;
  };
};

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

void GPIOPollingInterrupt(){
  
  auto itter = DEFINED_BUTTONS.begin();
    while(itter != DEFINED_BUTTONS.end()){
      GPIO_STATE[*itter] = digitalRead(*itter);
      itter++;
    };
  button_pressed = true;
};

void GPIOInterruptHandler_SINGULAR(){
  if((digitalRead(RETURN_GPIO) == LOW) && holding){

    if(holdCounting &&  time_reached(DeBounce)){
      destruct_alarm = true;
    
      if( (!button_pressed)){
        GPIO = RETURN_GPIO;
        button_pressed = true;
      };
    };
    return; 

  } else if(digitalRead(RETURN_GPIO) == HIGH) {
      if(time_reached(DeBounce) && !holdCounting){
        
        construct_alarm = true; 
      } else if((!holdCounting)){
        
        
      };
      

    return;
  }else {
    if(time_reached(DeBounce)){
      if (digitalRead(SELECT_GPIO) == HIGH){
        button_pressed = true;
        GPIO = SELECT_GPIO;
        return;
      }
      else if(digitalRead(NEXT_GPIO) == HIGH){
        button_pressed = true;
        GPIO = NEXT_GPIO;
        return;
      };
  
    };
  };
};


int attachGPIOPollingInterrupt(){
  attachInterrupt(digitalPinToInterrupt(9), GPIOPollingInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), GPIOPollingInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), GPIOPollingInterrupt, CHANGE);
  return 0;
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
  construct_alarm = false;
  holdCounting = true;
  holding = true;
  homeButtonAlarmID = createTimeout_single(alarm_pool_secondary, 1000, alarm_callback, (void*)&holding, true);
};

void cancelHoldAlarm(){
  destruct_alarm = false;
  holdCounting = false;
  holding = false;
  alarm_pool_cancel_alarm(alarm_pool_secondary, homeButtonAlarmID);
  homeButtonAlarmID = 0;
};





void Supervisor::GPIOInterruptHandler_RETURN(){
  uiSupervisor._trigger_return();
}

void Supervisor::GPIOInterruptHandler_SELECT(){
  uiSupervisor._trigger_select();
}

void Supervisor::GPIOInterruptHandler_NEXT(){
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
  construct_alarm = false;
  destruct_alarm = false;
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
  //logFunctionResult("Attach Supervisor interrupt", attachSupervisorInterruptSingular);
  logFunctionResult("Attach GPIOPolling interrupt", attachGPIOPollingInterrupt);
  
  
  uiSupervisor.startup();
  DeBounce = make_timeout_time_ms(DEBOUNCE);
};

void oldHandler(){
 if(button_pressed){
      button_pressed = false;  
      if(time_reached(DeBounce)){
        Serial.print("button_pressed[GPIO]: ");
        Serial.print(std::to_string(GPIO).c_str());
        Serial.print("@"); 
        Serial.print(std::to_string(digitalRead(GPIO)).c_str());
        Serial.println(" ");
        if(GPIO > 0){
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
        };
      
      };
      DeBounce = make_timeout_time_ms(DEBOUNCE);
    };
  
};

void loop() {
  
  while(!(time_reached(printFreq) || button_pressed || timer_fired || construct_alarm || destruct_alarm)){};
  if(timer_fired){
    timer_fired = false;
    cancelHoldAlarm();
    Serial.println("LONG PRESS!");
    uiSupervisor.HOME.trigger_function();
    button_pressed = false;
  } else if(time_reached(printFreq)){
    //Serial.println("test");
    printFreq = make_timeout_time_ms(1000);
  } else if(alivePacket.outstandingPrint){
    alivePacket.outstandingPrint = false;
    Serial.println(alivePacket.message.c_str());
  } else if(button_pressed){
    noInterrupts();
    bool duplicate[32] = {false};
    for (size_t i = 0; i < 32; i++)
    {
      duplicate[i] = GPIO_STATE[i];
    };
    interrupts();
    if((duplicate[RETURN_GPIO] == LOW) && holdCounting){
      cancelHoldAlarm();
      holdCounting = false;
      if(time_reached(DeBounce)){
        Serial.println("return released");
        uiSupervisor.REQUIRED_BUTTONS.RETURN.trigger_function();
      };
      
    }else if(duplicate[RETURN_GPIO] == HIGH){
      setupHoldAlarm();
      DeBounce = make_timeout_time_ms(15);
      button_pressed = false;
    }else if(time_reached(DeBounce)){
      if(duplicate[SELECT_GPIO] == HIGH){
        Serial.println("SELECT PRESSED");
        uiSupervisor.REQUIRED_BUTTONS.SELECT.trigger_function();
      }else if(duplicate[NEXT_GPIO] == HIGH){
        Serial.println("NEXT PRESSED");
        uiSupervisor.REQUIRED_BUTTONS.NEXT.trigger_function();
      }
    };
    if(button_pressed){
      DeBounce = make_timeout_time_ms(DEBOUNCE);
      button_pressed = false;
    };
  };
  uiSupervisor.run();
};

  

  
    
    