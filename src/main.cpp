#include <Arduino.h>
#include <pico.h>
#include <pico/stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <Wire.h>
#include "pico/util/queue.h"
#include "Supervisor.h"
#include "OLEDUIDisplay.h"
#include "TimerProgram.h"
#include "AutoLoginProgram.h"

OLEDUIDisplay scrrenObj;
/*
#include "Supervisor.h"
BasicRequiredInfo BASE_INFO("DESKTO-PICO", 0.2, "elemat28");
Supervisor supervisor(BASE_INFO);
*/

// Buttons
#define HOME_BUTTON -1
#define RETURN_GPIO 9
#define SELECT_GPIO 8
#define NEXT_GPIO 7
#define GPIO_DEBOUCE_MS 15
// Timer Pools
alarm_pool_t *alarm_pool_primary;
alarm_pool_t *alarm_pool_secondary;
alarm_pool_t *alarm_pool_destroyable;

absolute_time_t DeBounce;
alarm_id_t hold_callbackID;
// put function declarations here:
bool noUpdate = false;
bool printMsg = false;
bool recordedRead = true;
String output = String();
queue_t pendingWorkQueue;
queue_t interruptQueue;
int pinConfig(void);
int setupInitialAlarmPool();
int displaySetup();
int addPrograms();
int passDataToPrograms();
int serialSetup();
int threadSafeQueues();
int attachInterrupts();
int64_t hold_callback(alarm_id_t id, void *user_data);
void gpio_callback(uint gpio, uint32_t events);
Supervisor supervisor = Supervisor();
String message = String("Hello");
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.println("Serial1");
  Serial1.println("-------------------------------");
  hold_callbackID = (int32_t)-1;
  pinConfig();
  setupInitialAlarmPool();
  supervisor.assign_alarm_pool(alarm_pool_destroyable);
  threadSafeQueues();
  displaySetup();
  addPrograms();
  attachInterrupts();

  // scrrenObj.init();
  Serial1.println("screen int'd");
  // scrrenObj.safe_output(message.c_str());
  supervisor.finalize();
  supervisor.startup_finish();
  supervisor.run();
}

byte value = 0;
PendingWork queueOutput;
void loop()
{
  queue_remove_blocking(&pendingWorkQueue, &queueOutput);
  switch (queueOutput.TYPE)
  {
  case BUTTON:
    switch (queueOutput.PENDING_OBJECT_ID)
    {
    case HOME_BUTTON:
      supervisor.return_to_menu();
      supervisor.run(true);
      noUpdate = true;
      break;
    case RETURN_GPIO:
      printf("RETURN RELEASED \n");
      supervisor.REQUIRED_BUTTONS.RETURN.trigger_function();
      break;
    case SELECT_GPIO:
      printf("SELECT PRESSED \n");
      supervisor.REQUIRED_BUTTONS.SELECT.trigger_function();
      break;
    case NEXT_GPIO:
      printf("NEXT PRESSED \n");
      supervisor.REQUIRED_BUTTONS.NEXT.trigger_function();
      break;

    default:
      break;
    }
    break;
  case REFRESH:
    supervisor.run(true);
    break;
  default:
    break;
  };
  supervisor.run();
}

int pinConfig(void)
{
  pinMode(RETURN_GPIO, INPUT_PULLUP);
  pinMode(SELECT_GPIO, INPUT_PULLUP);
  pinMode(NEXT_GPIO, INPUT_PULLUP);
  return 0;
}

int displaySetup()
{
  supervisor.splashScreenDuringStartup(false);
  supervisor.startup_begin();
  scrrenObj = OLEDUIDisplay();
  supervisor.set_UIDisplay(&scrrenObj);
  return 0;
}

int attachInterrupts()
{
  gpio_set_irq_enabled_with_callback(9, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(8, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(7, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  return 0;
}

void gpio_callback(uint gpio, uint32_t events)
{

  if (!time_reached(DeBounce))
  {

    return;
  };

  // Put the GPIO event(s) that just happened into event_str
  // so we can print it
  PendingWork newStruct;
  bool buttonState;
  switch (gpio)
  {
  case RETURN_GPIO:
    if (events == GPIO_IRQ_EDGE_RISE)
    {
      if (hold_callbackID > 0)
      {
        alarm_pool_cancel_alarm(alarm_pool_secondary, hold_callbackID);
        hold_callbackID = 0;
      };
      buttonState = false;
      if (noUpdate)
      {
        noUpdate = false;
      }
      else
      {
        newStruct = PendingWork(BUTTON, gpio, &buttonState);
      }
    }
    else
    {
      if (hold_callbackID > 0)
      {
        alarm_pool_cancel_alarm(alarm_pool_secondary, hold_callbackID);
        hold_callbackID = 0;
      };
      hold_callbackID = alarm_pool_add_alarm_in_ms(alarm_pool_secondary, 1000, hold_callback, &pendingWorkQueue, true);
    };
    break;
  case SELECT_GPIO:
    if (events == GPIO_IRQ_EDGE_FALL)
    {
      buttonState = true;
      newStruct = PendingWork(BUTTON, gpio, &buttonState);
    };
    break;
  case NEXT_GPIO:
    if (events == GPIO_IRQ_EDGE_FALL)
    {
      buttonState = true;
      newStruct = PendingWork(BUTTON, gpio, &buttonState);
    };
    break;
  default:
    break;
  };
  if (newStruct.TYPE != NONE)
  {
    queue_try_add(&pendingWorkQueue, (void *)&newStruct);
  };
  DeBounce = make_timeout_time_ms(GPIO_DEBOUCE_MS);
  // printf("GPIO %d %s\n", gpio, event_str);
}

int64_t hold_callback(alarm_id_t id, void *user_data)
{
  PendingWork newStruct = PendingWork(BUTTON, HOME_BUTTON);
  queue_t *ptr = (queue_t *)user_data;
  queue_try_add(ptr, &newStruct);
  return 0;
};

int setupInitialAlarmPool()
{
  alarm_pool_primary = alarm_pool_create(1, 16);
  alarm_pool_secondary = alarm_pool_create(2, 32);
  alarm_pool_destroyable = alarm_pool_create(0, 64);
  return 0;
};

int threadSafeQueues()
{
  queue_init(&interruptQueue, sizeof(uint), 4);
  queue_init(&pendingWorkQueue, sizeof(PendingWork), 4);
  supervisor.set_workQueue(&pendingWorkQueue);

  return 0;
}

int addPrograms()
{
  supervisor.add_program(new TimerProgram, sizeof(TimerProgram));
  supervisor.add_program(new AutoLoginProgram, sizeof(AutoLoginProgram));
  return 0;
};