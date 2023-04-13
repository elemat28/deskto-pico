#ifndef PICOUILCD_H
#define PICOUILCD_H

#include <LiquidCrystal_I2C.h>
#include <string>
//To help us illustrate unit testing in action we will be making an object that 
//will encapsulate behavior of a setting for us 
//(think - software sound volume; max value 100, min 0, can't cross those boundries)

class LCDui
{
private:
    LiquidCrystal_I2C* LCD;
    LiquidCrystal_I2C obj;
    int _i2cAdrrs; 
    bool _backlight;
    //While there is no use in a setting you can't know the value of
    //we cannot allow another object to change our value without us even knowing
    //so we will add interface methods
    float _value_limit_min;
    float _value_limit_max;
    float _value_step;
public:
    LCDui();
    LCDui(int I2Caddress);
    ~LCDui();
    LiquidCrystal_I2C getLC(int I2Caddress);
    void init();
    void setBacklightEnabled(bool on);
    bool isBacklightOn();
    void clear();
    void printFromStart(const char* message);

};


#endif