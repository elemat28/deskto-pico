#include "UIButton.h"

void UIButton::setID(std::string ID){
  _ID = ID;
}

UIButton::UIButton(){
  _enabled = true;
  _displayAsValue = false;

}

UIButton::UIButton(std::string ID): UIButton(){
  _displayAs = ID;
  _ID = ID;
}

UIButton::UIButton(float* value){
  _displayAsValue = true;
  _value = value; 
}

UIButton::~UIButton(){

}

std::string UIButton::get_ID(){
  return _ID;
}


bool UIButton::enable(void){
  _enabled = true;
  return true;
}

bool UIButton::isEnabled(void){
  return _enabled;
}

void UIButton::shouldDisplayAsValue(bool displayAsCurrentValue){
  _displayAsValue = displayAsCurrentValue;
}

void UIButton::setValue(float* value){
  _value = value;
}

void UIButton::setCallback(void (*funct_ptr)(), void* target){
  _callback = funct_ptr;
}

void UIButton::setCallbackFunction(std::function<void()> callbackFunction){
  functionPointer = callbackFunction;
}


void UIButton::setDisplayAs(std::string newLabel){
  _displayAs = newLabel;
}

std::string UIButton::print(){
  if(!(_enabled)){
    return std::string();
  }
  if(_displayAsValue){
    return std::to_string(*_value);
  } else {
    return _displayAs;
  }
}

void UIButton::trigger(){
  
  if(_enabled){
    (*_callback)();
  }
  
}

void UIButton::trigger_function(){
  
  if(_enabled){
    functionPointer();
  }
}