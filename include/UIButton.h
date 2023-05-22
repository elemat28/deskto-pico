#ifndef PICOUIBUTTON_H
#define PICOUIBUTTON_H


#include <functional>
#include <string>
class UIButton
{
private:
    bool _enabled;
    bool _displayAsValue;
    std::string _displayAs;
    volatile float* _value;
    void (*_callback)(void);
    std::string _ID;
    std::function<void()> functionPointer; 
public:
    UIButton();
    UIButton(std::string ID);
    UIButton(float* value);
    ~UIButton();
    std::string get_ID();
    bool enable(void);
    bool isEnabled(void);
    void shouldDisplayAsValue(bool displayAsCurrentValue);
    void setValue(float* value);
    void setCallback(void (*funct_ptr)(), void* target);
    void setCallbackFunction(std::function<void()> callbackFunction);
    void setDisplayAs(std::string newLabel);
    std::string print();
    void trigger(void);
    void trigger_function(void);
    void setID(std::string ID);
    

};




#endif