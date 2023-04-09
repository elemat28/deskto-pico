#include "TempLib.h"

TempLib::TempLib(const char* displayableName, int ID)
{
    _ID = ID;
    _displayableName = displayableName;
    _value = 0;
    _value_limit_min = 0;
    _value_limit_max = 100;
    _value_step = 0.2;

}

TempLib::~TempLib()
{
}

 const char* TempLib::getDisplayableName() {return _displayableName;};

float TempLib::getCurrentValue(){return _value;};
float TempLib::getMax(){return _value_limit_max;};
float TempLib::getMin(){return _value_limit_max;};
bool TempLib::setTo(float value){return false;}; //returns true if error
bool TempLib::increase(){return false;};
bool TempLib::decrease(){return false;};