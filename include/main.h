#ifndef _MAIN_H
#define _MAIN_H
#include <Arduino.h>
#include "LCUIDisplay.h"
#include "UIButton.h"
#include "GPIOPolling.h"
#include "RotaryEncoder.h"
#include "Timer.h"
#include "Supervisor.h"
LCUIDisplay screen;
Supervisor uiSupervisor;
UIButton* buttonsArray;
RotaryEncoder rotary;
bool ledState;
struct pinButton;
pinButton* pinDictionary;
#endif