#include "main.h"
alarm_pool_t* sample;

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

void pinSetup(){
  Serial.println("Pin setup...");
  //RGB led
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);
  //encoder
  pinMode(12, OUTPUT); //CLICK
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  //button
  pinMode(9, INPUT_PULLDOWN); //CLICK
  pinMode(8, INPUT_PULLDOWN);
  pinMode(7, INPUT_PULLDOWN);
  //buzzer
  pinMode(6, OUTPUT);
  Serial.println("Pin setup complete!");
}

void defineButtons(){
  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    UIButtonsArray[i] = UIButton();
  }
}
volatile int  userdata = 0;
int last = 0;
repeating_timer rtInst;

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

void interruptFunction(void){

  for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    pinButtonDict[i].pinReading = digitalRead(pinButtonDict[i].GPIO);
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
std::string printe;
absolute_time_t timeStamp;
int ledGpi = 13;




void setup() {
  Serial.println("Setup Starting...");
  Serial.begin(9600);
  Serial.println("Waiting for connection!");
  while(!Serial && (millis()<1500));
  pinSetup();
  ledState = false;
  Serial.println("Connected");
  screen = LCDui(0x27);
  Serial.println("LCDui init");
  screen.init();
  Serial.println("LCD SETUP complete...");
  defineButtons();
  assignButtons();
    Serial.println("createAlarmPool...");
  sample = createAlarmPool();
  Serial.println("created!");
  createTimeout(sample, 25000,callbackFunct,(void*)pinButtonDict, (repeating_timer_t*)&rtInst);
  printe = std::string();
  digitalWrite(ledGpi, 1);
  timeStamp = make_timeout_time_ms(500);
  attachInterrupt(9, interruptFunction,  CHANGE);
  attachInterrupt(8, interruptFunction, CHANGE);
  attachInterrupt(7, interruptFunction, CHANGE);
}


void loop() {
  if(time_reached(timeStamp)){
    digitalWrite(ledGpi, !digitalRead(ledGpi));
    timeStamp = make_timeout_time_ms(500);
  }
   for (int i = 0; i < NUM_OF_DECLARED_PIN_BUTTONS; i++)
  {
    if((pinButtonDict[i].debounceMs == 0 ) || (time_reached(debounceTimestamps[i])))
    {
      pinButton dupe = pinButtonDict[i];
      if(dupe.pinReading > 0){
        Serial.print(dupe.HW_BUTTON_ID.c_str());
        Serial.println(std::to_string(dupe.pinReading).c_str());
        pinButtonDict[i].clicked = false;
        screen.printFromStart(dupe.HW_BUTTON_ID.c_str());
        debounceTimestamps[i] = make_timeout_time_ms(pinButtonDict[i].debounceMs);
      }
    }
  }

    
}