#include "UIButton.h"


UIButton::UIButton(){
  _enabled = false;
  _displayAsValue = false;

}

UIButton::UIButton(std::string label){
  _displayAsValue = false;
  _displayAs = label;
}

UIButton::UIButton(float* value){
  _displayAsValue = true;
  _value = value; 
}

UIButton::~UIButton(){

}

void UIButton::shouldDisplayAsValue(bool displayAsCurrentValue){
  _displayAsValue = displayAsCurrentValue;
}

void UIButton::setValue(float* value){
  _value = value;
}

void UIButton::setCallback(void* funct_ptr){
  _callback = funct_ptr;
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