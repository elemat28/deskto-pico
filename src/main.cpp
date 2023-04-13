#include "main.h"
alarm_pool_t* sample;

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
  pinMode(9, INPUT); //CLICK
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
volatile int userdata = 0;
int last = 0;
repeating_timer rtInst;

bool callbackFunct(repeating_timer* rt){
  *((int*)rt->user_data) = digitalRead(9);
  return true;
}

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
    Serial.println("createAlarmPool...");
  sample = createAlarmPool();
  Serial.println("created!");
  createTimeout(sample, 50000,callbackFunct,(void*)&userdata, (repeating_timer_t*)&rtInst);
  
 
}


void loop() {
  if(userdata != last){
    last = userdata;
    Serial.println(last);

    screen.printFromStart(std::to_string(last).c_str());
  }
}