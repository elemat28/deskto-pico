#include "LCUIDisplay.h"

LCUIDisplay::LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();

LCUIDisplay::LCI2CDisplay_t::LCI2CDisplay_t(int I2Caddress, int columns, int lines){
    I2Caddress = I2Caddress;
    columns = columns;
    lines = lines;
};

LCUIDisplay::LCI2CDisplay_t::LCI2CDisplay_t(){
    I2Caddress = 0x27;
    columns = 16;
    lines = 2;
};

//LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();
LCUIDisplay::LCUIDisplay():screen(DEFAULTDISPLAYCONFIG.I2Caddress, DEFAULTDISPLAYCONFIG.columns, DEFAULTDISPLAYCONFIG.lines){
  _backlight = false;
};


LCUIDisplay::LCUIDisplay(LCI2CDisplay_t displayDefinition):LCUIDisplay(){
  
  


};

int LCUIDisplay::init(){
  screen.init();
  screen.backlight();
  return 0;
};

void LCUIDisplay::clear(){
  screen.clear();
};

void LCUIDisplay::safe_output(char* data){
  screen.clear();
  screen.setCursor(0,0);
  screen.print(data);
}

void LCUIDisplay::safe_output(const char* data){
  safe_output((char*)data);
}

