#ifndef _MAIN_H
#define _MAIN_H
#include <Arduino.h>
#include "LCDui.h"
#include "UIButton.h"
#include "GPIOPolling.h"
#include "RotaryEncoder.h"
#include "Timer.h"
UIButton* buttonsArray;
LCDui screen;
RotaryEncoder rotary;
bool ledState;
struct pinButton;
pinButton* pinDictionary;
#endif