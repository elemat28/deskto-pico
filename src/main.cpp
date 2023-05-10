#include <Arduino.h>
#include <pico.h>
#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "LCUIDisplay.h"
#include "Supervisor.h"
#include "TimerProgram.h"
#define STDIO_UART 1
#define FLAG_VALUE 123

// Buttons
#define HOME_BUTTON -1
#define RETURN_GPIO 9
#define SELECT_GPIO 8
#define NEXT_GPIO 7
#define GPIO_DEBOUCE_MS 15
// Timer Pools
alarm_pool_t *alarm_pool_primary;
alarm_pool_t *alarm_pool_secondary;

absolute_time_t DeBounce;
// put function declarations here:
bool noUpdate = false;
bool printMsg = false;
bool recordedRead = true;
std::string output = std::string();
queue_t pendingWorkQueue;
queue_t interruptQueue;

LCUIDisplay display;

int myFunction(int, int);
int pinConfig(void);
int setupInitialAlarmPool();
int displaySetup();
int addPrograms();
int serialSetup();
int threadSafeQueues();
int attachInterrupts();
void gpio_callback(uint gpio, uint32_t events);
alarm_id_t hold_callbackID;

int64_t hold_callback(alarm_id_t id, void *user_data);
void core1_entry();
BasicRequiredInfo BASE_INFO("DESKTO-PICO", 0.2, "elemat28");
Supervisor supervisor(BASE_INFO);
int setupInitialAlarmPool();

void setup()
{
  supervisor.startup_begin();
  hold_callbackID = (int32_t)-1;
  output.reserve(64 * sizeof(char));
  pinConfig();
  displaySetup();
  addPrograms();
  serialSetup();
  setupInitialAlarmPool();
  threadSafeQueues();
  attachInterrupts();
  supervisor.finalize();
  supervisor.startup_finish();
  supervisor.run();
  DeBounce = make_timeout_time_ms(GPIO_DEBOUCE_MS);
}
PendingWork queueOutput;

void loop()
{

  // digitalRead(SELECT_GPIO);
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
  // supervisor.run();
};
// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
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
  display = LCUIDisplay(LCUIDisplay::DEFAULTDISPLAYCONFIG);
  display.init();
  supervisor.set_UIDisplay(&display);
  return 0;
}

int serialSetup()
{
  Serial1.begin(115200);
  Serial1.println("Hello, Raspberry Pi Pico!");
  return 0;
}

int threadSafeQueues()
{
  queue_init(&interruptQueue, sizeof(uint), 4);
  queue_init(&pendingWorkQueue, sizeof(PendingWork), 4);
  supervisor.set_workQueue(&pendingWorkQueue);

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

int addPrograms()
{
  supervisor.add_program(new TimerProgram, sizeof(TimerProgram));

  return 0;
};
void core1_entry()
{

  multicore_fifo_push_blocking(FLAG_VALUE);

  uint32_t g = multicore_fifo_pop_blocking();

  if (g != FLAG_VALUE)
    printf("Hmm, that's not right on core 1!\n");
  else
    printf("Its all gone well on core 1!");

  while (1)
    tight_loop_contents();
}

int setupInitialAlarmPool()
{
  alarm_pool_primary = alarm_pool_create(1, 16);
  alarm_pool_secondary = alarm_pool_create(2, 32);
  return 0;
}
