#include "main.h"
#define ALIVEMSGFREQSECS 2
#define MENUHOLD_MS 1500
#define DEBOUNCE 120

#define RETURN_GPIO 9
#define SELECT_GPIO 8
#define NEXT_GPIO   7

volatile int pollingAlarmID = 0;
volatile bool GPIO_STATE[32] = {false};
std::array< bool, 32> GPIO_ARRAY = {false};
volatile bool (*GPIO_STATE_PTR)[32] = &GPIO_STATE;
const std::vector<int> DEFINED_BUTTONS = {RETURN_GPIO, SELECT_GPIO, NEXT_GPIO};
absolute_time_t bounceBuffer;
std::vector<std::pair<int*, bool>> CURRENT_READINGS;
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
  if(*ptr && (digitalRead(RETURN_GPIO)==PinStatus::LOW)){
    timer_fired = true;
  };
  holdCounting = false;
  holding = false;

  return 0;
};

int64_t polling_alarm_callback(alarm_id_t id, void *user_data) {
  const int threshold = 5;
  
  if(!button_pressed){
  auto itter = DEFINED_BUTTONS.begin();
    while(itter != DEFINED_BUTTONS.end()){
      int low_readings = 0;
      int high_readings = 0;
      while((high_readings < threshold)&&(low_readings < threshold)){
        if(digitalRead(*itter) == HIGH){
          high_readings++;
        }else if(digitalRead(*itter) == LOW){
          low_readings++;
        };
      };
      if(high_readings >= threshold){
        GPIO_STATE[*itter] = HIGH;
      } else {
        GPIO_STATE[*itter] = LOW;
      };     
      itter++;
    };
  button_pressed = true;
  };
  
  return 0;
};


void polling_alarm_callback_dual(){
  absolute_time_t timeDelay = make_timeout_time_ms(1);
  busy_wait_until(timeDelay);
  std::array< bool, 32> GPIO_STATE_POLLING_ARRAY = {false};
  if(digitalRead(RETURN_GPIO) == LOW){
      if(!holdCounting){
        setupHoldAlarm();
        
      };
      
      
  }else if(holdCounting && (digitalRead(RETURN_GPIO)==HIGH)){
    timeDelay = make_timeout_time_ms(1);
    busy_wait_until(timeDelay);
    GPIO_STATE_POLLING_ARRAY[RETURN_GPIO] = HIGH;
    button_pressed = true;
  }else if(digitalRead(SELECT_GPIO) == LOW){
    timeDelay = make_timeout_time_ms(1);
    busy_wait_until(timeDelay);
          GPIO_STATE_POLLING_ARRAY[SELECT_GPIO]  = HIGH;
          button_pressed = true;
  }else if(digitalRead(NEXT_GPIO) == LOW){
    timeDelay = make_timeout_time_ms(1);
    busy_wait_until(timeDelay);
          GPIO_STATE_POLLING_ARRAY[NEXT_GPIO]  = HIGH;
          button_pressed = true;
  };
  if(button_pressed == true){
    GPIO_ARRAY = GPIO_STATE_POLLING_ARRAY;
    if(holdCounting){
      cancelHoldAlarm();
    }
  }
  timeDelay = make_timeout_time_ms(1);
  busy_wait_until(timeDelay);
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
  pinMode(9, INPUT_PULLUP); //CLICK
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
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
  if(!button_pressed){
  auto itter = DEFINED_BUTTONS.begin();
    while(itter != DEFINED_BUTTONS.end()){
      GPIO_STATE[*itter] = digitalRead(*itter);
      
      itter++;
    };
  button_pressed = true;
  };
};

void GPIOPollingStarter(){
  if(!button_pressed){
  button_pressed = true;
  };
};

void DelayedPollingStarter(){
  if((pollingAlarmID == 0)){
  pollingAlarmID = alarm_pool_add_alarm_in_ms(alarm_pool_secondary, 1, polling_alarm_callback, (void*)&holding, true);
  
  };
  
};


void GPIOInterrupt_Return(){
  if(time_reached(DeBounce)){
    uiSupervisor.REQUIRED_BUTTONS.RETURN.trigger_function();
    button_pressed = true;
  };
};

void GPIOInterrupt_Select(){
  if(time_reached(DeBounce)){
    uiSupervisor.REQUIRED_BUTTONS.SELECT.trigger_function();
    button_pressed = true;
  };
};
void GPIOInterrupt_Next(){
  if(time_reached(DeBounce)){
    uiSupervisor.REQUIRED_BUTTONS.NEXT.trigger_function();
    button_pressed = true;
  };
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

int attachPerButtonInterrupt(){
  attachInterrupt(digitalPinToInterrupt(RETURN_GPIO), GPIOInterrupt_Return, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SELECT_GPIO), GPIOInterrupt_Select, CHANGE);
  attachInterrupt(digitalPinToInterrupt(NEXT_GPIO),   GPIOInterrupt_Next,   CHANGE);
  return 0;
};

int attachDualPollingStarter(){
  attachInterrupt(digitalPinToInterrupt(9), polling_alarm_callback_dual, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), polling_alarm_callback_dual, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), polling_alarm_callback_dual, CHANGE);
  return 0;
};
int attachDelayedPollingStarter(){
  attachInterrupt(digitalPinToInterrupt(9), DelayedPollingStarter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), DelayedPollingStarter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), DelayedPollingStarter, CHANGE);
  return 0;
};

int attachGPIOPollingStarter(){
  attachInterrupt(digitalPinToInterrupt(9), GPIOPollingStarter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), GPIOPollingStarter, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), GPIOPollingStarter, CHANGE);
  return 0;
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
  
  //hardware_alarm_claim(3);
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
  bounceBuffer = make_timeout_time_ms(5);
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
  pollingAlarmID = 0;
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
  bounceBuffer = get_absolute_time();
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
  //logFunctionResult("Attach GPIOPolling interrupt", attachGPIOPollingInterrupt);
  //logFunctionResult("Attach attachGPIOPollingStarter", attachGPIOPollingStarter);
  //logFunctionResult("Attach PerButtonInterrupts", attachPerButtonInterrupt);
  //logFunctionResult("Attach DelayedPollingStarter", attachDelayedPollingStarter);
  logFunctionResult("Attach attachDualPollingStarter", attachDualPollingStarter);
  
  uiSupervisor.startup();
  DeBounce = make_timeout_time_ms(DEBOUNCE);
};


void loop() {
  
  while(!(time_reached(printFreq) || button_pressed || timer_fired || construct_alarm || destruct_alarm)){};
  if(timer_fired){
    Serial.println("LONG PRESS!");
    timer_fired = false;
    cancelHoldAlarm();
    uiSupervisor.HOME.trigger_function();
    button_pressed = false;
  } else if(time_reached(printFreq)){
    printFreq = make_timeout_time_ms(1000);
  } else if(alivePacket.outstandingPrint){
    alivePacket.outstandingPrint = false;
    Serial.println(alivePacket.message.c_str());
  } else if(button_pressed){
    if(time_reached(DeBounce)){
      if(GPIO_ARRAY[RETURN_GPIO]){
        Serial.print("[HOLD_COUNTING & RETURN_LOW]");
        uiSupervisor.REQUIRED_BUTTONS.RETURN.trigger_function();
      }else if(GPIO_ARRAY[SELECT_GPIO]){
        Serial.print("SELECT_HIGH");
        uiSupervisor.REQUIRED_BUTTONS.SELECT.trigger_function();
      }else if(GPIO_ARRAY[NEXT_GPIO]){
        Serial.print("NEXT_HIGH");
        uiSupervisor.REQUIRED_BUTTONS.NEXT.trigger_function();
      }
      Serial.println("//");
      DeBounce = make_timeout_time_ms(DEBOUNCE);
    }
    
  };  
  
  //Serial.println("//");
  uiSupervisor.run();
  if(button_pressed){
    button_pressed = false;
  }
};

  

  
    
    