#ifndef _MAIN_H
#define _MAIN_H
#include <Arduino.h>
#include "LCUIDisplay.h"
#include "UIButton.h"
#include "GPIOPolling.h"
#include "RotaryEncoder.h"
#include "TimerProgram.h"
#include "Supervisor.h"

volatile bool holding;
volatile bool holdCounting;
volatile bool callbackResult;
volatile bool timer_fired;
volatile bool button_pressed;
volatile int  GPIO;
volatile bool construct_alarm;
volatile bool destruct_alarm;
LCUIDisplay screen;
Supervisor uiSupervisor;
UIButton* buttonsArray;
RotaryEncoder rotary;
bool ledState;
struct pinButton;
pinButton* pinDictionary;
alarm_pool_t* alarm_pool_primary;
alarm_pool_t* alarm_pool_secondary;
repeating_timer rtInst;
repeating_timer returnHomeTimer;
absolute_time_t timeStamp;
absolute_time_t returnHomeTimestamp;
absolute_time_t DeBounce;
absolute_time_t printFreq;
int homeButtonAlarmID;
int64_t alarm_callback(alarm_id_t id, void *user_data);
int logFunctionResult(const std::string functionMessage, int (*function_ptr)(), bool logToConsole);
int pinSetup();
int createUISupervisor();
int setupInitialAlarmPool();
bool repeatingPrintAliveFunct(repeating_timer* rt);
int setupMainMenuOnHold();
int setupAlivePrintToSerial();
void setupHoldAlarm();
void cancelHoldAlarm();
#endif