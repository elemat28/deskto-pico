#ifndef PICOUIBUTTON_H
#define PICOUIBUTTON_H


#include <string>
//To help us illustrate unit testing in action we will be making an object that 
//will encapsulate behavior of a setting for us 
//(think - software sound volume; max value 100, min 0, can't cross those boundries)

class UIButton
{
private:
    bool _enabled;
    bool _displayAsValue;
    std::string _displayAs;
    volatile float* _value;
    void* _callback;
    //While there is no use in a setting you can't know the value of
    //we cannot allow another object to change our value without us even knowing
    //so we will add interface methods

public:
    UIButton();
    UIButton(std::string label);
    UIButton(float* value);
    ~UIButton();
    void shouldDisplayAsValue(bool displayAsCurrentValue);
    void setValue(float* value);
    void setCallback(void* funct_ptr);
    void setDisplayAs(std::string newLabel);
    std::string print();

};




#endif