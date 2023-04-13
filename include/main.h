#ifndef _MAIN_H
#define _MAIN_H
#include <Arduino.h>
#include "LCDui.h"
#include "UIButton.h"
#include "GPIOPolling.h"
UIButton* buttonsArray;
LCDui screen;
bool ledState;
static int numOfUIButtons;
struct pinButton;
pinButton* pinDictionary;
#endif