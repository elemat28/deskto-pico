#include "RotaryEncoder.h"


RotaryEncoder::RotaryEncoder(){
  _value = 0;
}

RotaryEncoder::~RotaryEncoder(){
}

int RotaryEncoder::getValue(){
  return _value;
}

void RotaryEncoder::turnCW(){
  _value++;
}

void RotaryEncoder::turnACW(){
  _value--;
}