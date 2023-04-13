#include "main.h"
alarm_pool_t* sample;

struct pinButton{
  int GPIO;
  volatile bool clicked;
  UIButton* button;
};

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
  pinMode(9, INPUT_PULLUP); //CLICK
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  //buzzer
  pinMode(6, OUTPUT);
  Serial.println("Pin setup complete!");
}

void defineButtons(){
   Serial.println("Defining UI buttons...");
   UIButton generatedButtons[] = {
    UIButton()
    ,UIButton()
    ,UIButton()
    ,UIButton()
    ,UIButton()
    ,UIButton()
    };
   buttonsArray = generatedButtons;
   numOfUIButtons = sizeof(generatedButtons)/sizeof(UIButton);
   std::string msg = std::string("Defined ");
   msg.append(std::to_string(numOfUIButtons));
   msg.append(" buttons!");
   Serial.println(msg.c_str());
}
volatile int  userdata = 0;
int last = 0;
repeating_timer rtInst;

bool callbackFunct(repeating_timer* rt){
  pinButton* arrOf = (pinButton*)rt->user_data;
  if(0 == digitalRead((*arrOf).GPIO)){
    digitalWrite(15,1);
    (*arrOf).clicked = (volatile bool)false;
  }
 

  
  return true;
}

void assignButtons(){
  Serial.println("Assigning buttons to GPIOs...");
  pinButton yellow;
  yellow.GPIO = 9;
  yellow.clicked = false;
  yellow.button = buttonsArray;
  pinButton green;
  green.GPIO = 8;
  green.clicked = false;
  green.button = buttonsArray+(sizeof(UIButton));
  pinButton arr[] = {yellow, green};
  pinDictionary = arr;
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
  //createTimeout(sample, 25000,callbackFunct,(void*)pinDictionary, (repeating_timer_t*)&rtInst);
  printe = std::string();
  digitalWrite(ledGpi, 1);
  timeStamp = make_timeout_time_ms(500);
  
}


void loop() {
  pinButton* localptr = pinDictionary;
  if(localptr->GPIO == 9){
    Serial.println("Equal");
  }
  Serial.println(std::to_string(localptr->GPIO).c_str());

    
}