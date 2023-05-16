#include <Arduino.h>
#include <pico.h>
#include <stdio_ext.h>
#include <pico/stdio.h>
#include <Wire.h>
#include <Adafruit_TinyUSB.h>
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
uint8_t const desc_hid_report[] =
    {
        TUD_HID_REPORT_DESC_KEYBOARD()};
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

void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);
uint8_t keycode[6] = {0};
uint8_t count = 0;
#ifdef ARDUINO_ARCH_RP2040
uint8_t pins[] = {16, 17, 18, 19};
#else
uint8_t pins[] = {A0, A1, A2, A3};
#endif

// number of pins
uint8_t pincount = sizeof(pins) / sizeof(pins[0]);

// For keycode definition check out https://github.com/hathach/tinyusb/blob/master/src/class/hid/hid.h
uint8_t hidcode[] = {HID_KEY_ARROW_RIGHT, HID_KEY_ARROW_LEFT, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_UP};
static bool keyPressedPreviously = false;
#if defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(ARDUINO_NRF52840_CIRCUITPLAY) || defined(ARDUINO_FUNHOUSE_ESP32S2)
bool activeState = true;
#else
bool activeState = false;
#endif
Supervisor supervisor = Supervisor();
String message = String("Hello");
void core1_entry()
{
}
int x = 0;
void c1Entry()
{
  while (true)
  {

    delay(1000);
    digitalWrite(15, !digitalRead(15));
  }
}
Adafruit_USBD_HID usb_hid;
void setup()
{
  usb_hid = Adafruit_USBD_HID(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);
  // Serial.begin(115200);
  // Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.println("Serial1");
  Serial1.println("-------------------------------");
  hold_callbackID = (int32_t)-1;
  pinConfig();

  // multicore_launch_core1(c1Entry);
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
  usb_hid.setReportCallback(NULL, hid_report_callback);
  digitalWrite(15, HIGH);

  usb_hid.begin();
  while (!TinyUSBDevice.mounted())
  {
    delay(1);
    Serial1.println("NOT MTD\n");
  };
  digitalWrite(15, LOW);
  supervisor.run();
}

void type()
{
  int delay_ms = 10;
  usb_hid.keyboardPress(0, 'p');
  delay(delay_ms);
  usb_hid.keyboardRelease(0);
  delay(delay_ms);
  usb_hid.keyboardPress(0, 'a');
  delay(delay_ms);
  usb_hid.keyboardRelease(0);
  delay(delay_ms);
  usb_hid.keyboardPress(0, 's');
  delay(delay_ms);
  usb_hid.keyboardRelease(0);
  delay(delay_ms);
  usb_hid.keyboardPress(0, 's');
  delay(delay_ms);
  usb_hid.keyboardRelease(0);
  delay(delay_ms);
}

byte value = 0;
PendingWork queueOutput;
void loop()
{

  while (true)
  {
    // poll gpio once each 2 ms
    delay(2);

    // used to avoid send multiple consecutive zero report for keyboard
    static bool keyPressedPreviously = false;

    uint8_t count = 0;
    uint8_t keycode[6] = {0};

    // scan normal key and send report
    for (uint8_t i = 0; i < pincount; i++)
    {
      if (activeState == digitalRead(pins[i]))
      {
        if (pins[i] == 17)
        {
          type();
        }
        else
        {
          // if pin is active (low), add its hid code to key report
          keycode[count++] = hidcode[i];
        };
        // 6 is max keycode per report
        if (count == 6)
          break;
      }
    }

    if (TinyUSBDevice.suspended() && count)
    {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host
      Serial1.println("WAKEUP\n");
      TinyUSBDevice.remoteWakeup();
    }

    // skip if hid is not ready e.g still transferring previous report
    if (!usb_hid.ready())
    {

      digitalWrite(14, HIGH);
      return;
    };
    digitalWrite(15, LOW);
    if (count)
    {
      Serial.println("cnt\n");
      // Send report if there is key pressed
      uint8_t const report_id = 0;
      uint8_t const modifier = 0;

      keyPressedPreviously = true;
      usb_hid.keyboardReport(report_id, modifier, keycode);
    }
    else
    {
      // Send All-zero report to indicate there is no keys pressed
      // Most of the time, it is, though we don't need to send zero report
      // every loop(), only a key is pressed in previous loop()
      if (keyPressedPreviously)
      {
        keyPressedPreviously = false;
        usb_hid.keyboardRelease(0);
      }
    }
  };

  Serial1.println("blocking \n");
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
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
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

void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
  (void)report_id;
  (void)bufsize;
  Serial.println("Dup4\n");
  // LED indicator is output report with only 1 byte length
  if (report_type != HID_REPORT_TYPE_OUTPUT)
    return;

  // The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
  // Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  uint8_t ledIndicator = buffer[0];
  // usb_hid.keyboardPress(0, 'A');
  //  turn on LED if capslock is set
  digitalWrite(LED_BUILTIN, ledIndicator & KEYBOARD_LED_CAPSLOCK);

#ifdef PIN_NEOPIXEL
  pixels.fill(ledIndicator & KEYBOARD_LED_CAPSLOCK ? 0xff0000 : 0x000000);
  pixels.show();
#endif
}

int addPrograms()
{
  supervisor.add_program(new TimerProgram, sizeof(TimerProgram));
  supervisor.add_program(new AutoLoginProgram, sizeof(AutoLoginProgram));
  return 0;
};