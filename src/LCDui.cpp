#include "LCDui.h"

//To help us illustrate unit testing in action we will be making an object that 
//will encapsulate behavior of a setting for us 
//(think - software sound volume; max value 100, min 0, can't cross those boundries)

LiquidCrystal_I2C LCDui::getLC(int I2Caddress){
  LiquidCrystal_I2C lcd(I2Caddress,16,2);
  return lcd;
}

LCDui::LCDui() : obj(0x27,16,2)
{
  _backlight = false;
 
}

LCDui::LCDui(int I2Caddress) : LCDui()
{
  _i2cAdrrs = I2Caddress;
  _backlight = false;
  Serial.println("PRELC");
  Serial.println("postLC");
  init();
  Serial.println("LCD");
  
  
}

LCDui::~LCDui()
{
    clear();
    setBacklightEnabled(false);
}

void LCDui::init(){
    obj.init();
    obj.backlight();
    _backlight = true;
}

void LCDui::setBacklightEnabled(bool on){
  _backlight = on;
  obj.setBacklight(on);

}

bool LCDui::isBacklightOn(){
  return _backlight;
}

void LCDui::clear(){
  obj.clear();
}

void LCDui::printFromStart(const char* message){
  obj.clear();
  obj.setCursor(0,0);
  obj.print(message);
}
