#ifndef TEMPLIB_H
#define TEMPLIB_H
//To help us illustrate unit testing in action we will be making an object that 
//will encapsulate behavior of a setting for us 
//(think - software sound volume; max value 100, min 0, can't cross those boundries)
class TempLib
{
private:
    const char* _displayableName; 
    int _ID;
    float _value;
    //While there is no use in a setting you can't know the value of
    //we cannot allow another object to change our value without us even knowing
    //so we will add interface methods
    float _value_limit_min;
    float _value_limit_max;
    float _value_step;
public:
    TempLib(const char* displayableName, int ID);
    ~TempLib();
    const char* getDisplayableName();
    float getCurrentValue();
    float getMax();
    float getMin();
    bool setTo(float value); //returns true if error
    bool increase();
    bool decrease();
};


#endif