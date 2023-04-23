#include "LCUIDisplay.h"

LCUIDisplay::LCI2CDisplay_t::LCI2CDisplay_t(int I2Caddress, int columns, int lines){
    I2Caddress = I2Caddress;
    columns = columns;
    lines = lines;
};


//LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();
LCUIDisplay::LCUIDisplay(): LCUIDisplay(DEFAULTLCI2CADDR){


}


LCUIDisplay::LCUIDisplay(int IC2Address){
  _backlight = false;
  _i2cAdrrs = IC2Address; 
  //obj()
   


}

void LCUIDisplay::safe_output(char* data){
}
