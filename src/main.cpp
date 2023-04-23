#include "main.h"
#define ALIVEMSGFREQSECS 2

alarm_pool_t* sample;
absolute_time_t timeStamp;
repeating_timer rtInst;
struct AlivePacket {
  
  std::string message;
  volatile bool outstandingPrint = false;
};

AlivePacket alivePacket;



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

int ScreenSetup(){
  screen = LCDui(0x27);
  screen.init();
  return 0;
}

int createUISupervisor(){
  uiSupervisor = Supervisor();
  return 0;
}

int finalizeSupervisor(){
  uiSupervisor.finalize();
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
    //uint32_t seconds = (to_ms_since_boot(get_absolute_time())/1000);
    (*dataPtr).message.clear();
    (*dataPtr).message.append("Runtime[seconds]: ");
    (*dataPtr).message.append(std::to_string((int)(time_us_64()/(1000*1000))).c_str());
    (*dataPtr).outstandingPrint = true;
  } 
  
  //packetInstance.outstandingPrint = true;
  //alivePacket* userData = (alivePacket*)(rt->user_data);
  //if(!userData->outstandingPrint){
    if(false){
    std::string temp = std::string("Running for ");
    temp.append(std::to_string((to_ms_since_boot(get_absolute_time())/1000)));
    temp.append(" Seconds");
    //userData->message.append(temp);
    //userData->outstandingPrint = true;
  };
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

void screenBacklightToggle(void){
  Serial.println("screenToggleCallback!");
  screen.setBacklightEnabled(!screen.isBacklightOn());
}

void logEncoderValue(void){
  Serial.println(std::to_string(rotary.getValue()).c_str());
}

void assignFunctionsToButtons(){
  Serial.println("assigning functions to buttons...");
  //YELLOW - toggle backlight
  pinButton YELLOW = pinButtonDict[0];
  YELLOW.button->setCallback(&screenBacklightToggle);
  YELLOW.button->enable();
  pinButtonDict[0] = YELLOW;

  //GREEN - Serial log encoder Val
  pinButton GREEN = pinButtonDict[1];
  GREEN.button->setCallback(&logEncoderValue);
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
        screen.printFromStart(dupe.HW_BUTTON_ID.c_str());
        debounceTimestamps[i] = make_timeout_time_ms(pinButtonDict[i].debounceMs);
      }
    }
  }  

}

void setup() {
  alivePacket.message.reserve(64); //without this timer breaks if string has to be resized in the callback
  
  Serial.begin(9600);
  logFunctionResult("Pin Setup", pinSetup);
  logFunctionResult("I2C 1602 LCD", ScreenSetup);
  logFunctionResult("UISupervisor init", createUISupervisor);
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
  logFunctionResult("Repeating ALIVE timer activation", setupAlivePrintToSerial);
  
}


void loop() {
  while(alivePacket.outstandingPrint == false){};
  if(alivePacket.outstandingPrint == true){
    Serial.println(alivePacket.message.c_str());
    alivePacket.outstandingPrint = false;
  }

 
  
  
  
}