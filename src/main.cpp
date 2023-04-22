#include "main.h"
alarm_pool_t* sample;
absolute_time_t timeStamp;
repeating_timer rtInst;
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
  pinMode(12, INPUT_PULLUP); //CLICK
  pinMode(11, INPUT);
  pinMode(10, INPUT);
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

void setup() {
  float timerMins = 1;
  Timer timer = Timer(timerMins);
  timer.start();
  Serial.println("Setup Starting...");
  Serial.begin(9600);
  Serial.println("Waiting for connection!");
  while(!Serial && (millis()<1500));
  pinSetup();
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
  assignFunctionsToButtons();
  createTimeout(sample, 25000,callbackFunct,(void*)pinButtonDict, (repeating_timer_t*)&rtInst);
  rotary = RotaryEncoder();
  timeStamp = make_timeout_time_ms(500);
  setupInterrupts();
  Serial.println(std::to_string(timer.getTimeLeftAsSeconds()).c_str());
}


void loop() {
  if(time_reached(timeStamp)){
    digitalWrite(28, !digitalRead(28));
    timeStamp = make_timeout_time_ms(500);
  }
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