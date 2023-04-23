#ifndef PICOUIBUTTON_H
#define PICOUIBUTTON_H


#include <string>
class UIButton
{
private:
    bool _enabled;
    bool _displayAsValue;
    std::string _displayAs;
    volatile float* _value;
    void (*_callback)(void);


public:
    UIButton();
    UIButton(std::string label);
    UIButton(float* value);
    ~UIButton();
    bool enable(void);
    bool isEnabled(void);
    void shouldDisplayAsValue(bool displayAsCurrentValue);
    void setValue(float* value);
    void setCallback(void (*funct_ptr)());
    void setDisplayAs(std::string newLabel);
    std::string print();
    void trigger(void);


};




#endif