#include "main.h"
#define ALIVEMSGFREQSECS 2
absolute_time_t DeBounce;
alarm_pool_t* sample;
absolute_time_t timeStamp;
repeating_timer rtInst;
struct AlivePacket {
  
  std::string message;
  volatile bool outstandingPrint = false;
};

AlivePacket alivePacket;

bool button_pressed = false;

long int itter;

volatile int data = 0;

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
}
struct pinButtonDeclaration {
  std::string HW_BUTTON_ID; 
  int GPIO;
};
struct pinButton{
  std::string HW_BUTTON_ID; 
  int GPIO;
  UIButton* button;
  int debounceMs = 50;
  volatile bool clicked = false;
  volatile int pinReading = 0;
};

pinButtonDeclaration declarationDict[] = {
  {std::string("BUTTON_YELLOW"), 9},
  {std::string("BUTTON_GREEN"), 8},
  {std::string("BUTTON_BLUE"), 7}
};

const int NUM_OF_DECLARED_PIN_BUTTONS = sizeof(declarationDict)/sizeof(pinButtonDeclaration);

UIButton UIButtonsArray[NUM_OF_DECLARED_PIN_BUTTONS];

pinButton pinButtonDict[NUM_OF_DECLARED_PIN_BUTTONS];

absolute_time_t debounceTimestamps[NUM_OF_DECLARED_PIN_BUTTONS];

void* programFunctionsDeclaration[] = {

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
}

int createUISupervisor(){
  
  return 0;
}

int createUIdisplay(){
  screen = LCUIDisplay();
  return 0;
}

int addDisplayToSupervisor(){
  uiSupervisor.set_UIDisplay(&screen);
  return 0;
}

int finalizeSupervisor(){
  uiSupervisor.finalize();
  return 0;
}

int startupSupervisor(){
  uiSupervisor.startup();
  return 0;
}

int attachSupervisorInterrupts(){
  attachInterrupt(digitalPinToInterrupt(9), Supervisor::GPIOInterruptHandler_RETURN, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), Supervisor::GPIOInterruptHandler_SELECT, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), Supervisor::GPIOInterruptHandler_NEXT, CHANGE);
  return 0;
}

int attachSupervisorInterruptSingular(){
  attachInterrupt(digitalPinToInterrupt(9), Supervisor::GPIOInterruptHandler_SINGULAR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), Supervisor::GPIOInterruptHandler_SINGULAR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(7), Supervisor::GPIOInterruptHandler_SINGULAR, CHANGE);
  return 0;
}

int setupInitialAlarmPool(){
  sample = createAlarmPool();
  return 0;
}

void defineButtons(){
  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    
    UIButtonsArray[i] = UIButton();
  }
}

bool callbackFunct(repeating_timer* rt){
  pinButton* arrOf = (pinButton*)rt->user_data;
  pinButton parr = arrOf[0];
  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    pinButton parr = arrOf[i];
    if(0 < digitalRead(parr.GPIO)){
    digitalWrite(15,1);
    parr.clicked = false;
    }
  }
  
  return true;
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
}

void interruptFunction(void){

  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    pinButtonDict[i].pinReading = digitalRead(pinButtonDict[i].GPIO);
  }
}

void RotaryInterrupt(void){
  if(digitalRead(11) == digitalRead(10)){
    rotary.turnACW();
  } else {
    rotary.turnCW();
  }

}

void assignButtons(){
  Serial.println("Assigning buttons to GPIOs...");
  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    pinButtonDict[i] = pinButton{declarationDict[i].HW_BUTTON_ID, declarationDict[i].GPIO, &(UIButtonsArray[i])};
    debounceTimestamps[i] = make_timeout_time_ms(pinButtonDict[i].debounceMs);
  }
  Serial.println("Buttons to GPIOs OK");
}

void assignFunctionsToButtons(){
  Serial.println("assigning functions to buttons...");
  //YELLOW - toggle backlight
  pinButton YELLOW = pinButtonDict[0];
  //YELLOW.button->setCallback(&screenBacklightToggle);
  YELLOW.button->enable();
  pinButtonDict[0] = YELLOW;

  //GREEN - Serial log encoder Val
  pinButton GREEN = pinButtonDict[1];
  //GREEN.button->setCallback(&logEncoderValue);
  GREEN.button->enable();
  pinButtonDict[1] = GREEN;
  Serial.println("OK!");
}



void setupInterrupts(){
  Serial.println("attaching interrupts...");
  //Buttons
  attachInterrupt(digitalPinToInterrupt(7), interruptFunction, CHANGE);
  attachInterrupt(digitalPinToInterrupt(8), interruptFunction, CHANGE);
  attachInterrupt(digitalPinToInterrupt(9), interruptFunction, CHANGE);

  //Rotary
  attachInterrupt(digitalPinToInterrupt(10), RotaryInterrupt, FALLING);
  Serial.println("OK!");
}

int setupAlivePrintToSerial(){
  if(createTimeout(sample, ALIVEMSGFREQSECS*1000000, repeatingPrintAliveFunct,(void*)&alivePacket, (repeating_timer_t*)&rtInst)){
    return 0;
  } else {
    return 1;
  }
}

void x(){
  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    if((pinButtonDict[i].debounceMs == 0 ) || (time_reached(debounceTimestamps[i])))
    {
      pinButton dupe = pinButtonDict[i];
      if(dupe.pinReading == HIGH){
        Serial.print(dupe.HW_BUTTON_ID.c_str());
        if(dupe.button->isEnabled()){
          Serial.print(" ENABLED ");
        } else {
          Serial.print(" NOT ENABLED ");
        }
        dupe.button->trigger();
        Serial.println(std::to_string(dupe.pinReading).c_str());
        pinButtonDict[i].clicked = false;
        debounceTimestamps[i] = make_timeout_time_ms(pinButtonDict[i].debounceMs);
      }
    }
  }  

}
void Supervisor::GPIOInterruptHandler_SINGULAR(){
 if(!time_reached(DeBounce)){
  return;
 }
 DeBounce = make_timeout_time_ms(25);
  if(digitalRead(9) == HIGH){
    button_pressed = true;
  uiSupervisor._trigger_return();
  }else if (digitalRead(8) == HIGH)
  {
  button_pressed = true;
  uiSupervisor._trigger_select();
  }else if (digitalRead(7) == HIGH)
  {
    button_pressed = true;
  uiSupervisor._trigger_next();
  }

  
  
}


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
  alivePacket.message.reserve(64); //without this timer breaks if string has to be resized in the callback
  Serial.begin(19200);
  DeBounce = make_timeout_time_ms(25);
  logFunctionResult("Pin Setup", pinSetup);
  //logFunctionResult("I2C 1602 LCD", ScreenSetup);
  logFunctionResult("UISupervisor init", createUISupervisor);
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
  logFunctionResult("Supervisor STARTUP", startupSupervisor);
  logFunctionResult("Repeating ALIVE timer activation", setupAlivePrintToSerial);
  //logFunctionResult("Attach Supervisor interrupt", attachSupervisorInterrupts);
  logFunctionResult("Attach Supervisor SINGLE interrupt",  attachSupervisorInterruptSingular);
 
  
}



void loop() {
  
  digitalWrite(15, LOW);
  while(!alivePacket.outstandingPrint && !uiSupervisor.peekhasWork()){};
  digitalWrite(15, HIGH);
  if(alivePacket.outstandingPrint == true){
    Serial.println(alivePacket.message.c_str());
    alivePacket.outstandingPrint = false;
  };
  if(uiSupervisor.peekhasWork()){
    //Serial.println("Has work");
    uiSupervisor.run();
    
  }
  

 
  
  
  
}