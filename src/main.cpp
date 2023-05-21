#include <Arduino.h>
#include <pico.h>
#include <stdio_ext.h>
#include <pico/stdio.h>
#include <Wire.h>
#include <Adafruit_TinyUSB.h>
#include "pico/util/queue.h"
#include "Supervisor.h"
#include "OLEDUIDisplay.h"
#include "LCUIDisplay.h"
#include "DEBUGSerialUIDisplay.h"
#include "TimerProgram.h"
#include "AutoLoginProgram.h"
#include "RgbLED.h"
#include "PicoBuzzer.h"

std::vector<SongNote> button_press = {
    SongNote(NOTE_G3, 50),
    // SongNote(NOTE_B3, 100)
};

std::vector<SongNote> happy_song = {
    SongNote(NOTE_F3, 250),
    SongNote(NOTE_E3, 250),
    SongNote(NOTE_G3, 250),
    SongNote(NOTE_A3, 250),
    // SongNote(NOTE_B3, 100)
};
OLEDUIDisplay scrrenObj;
LCUIDisplay LCScreen;
DEBUGSerialUIDisplay serialDisplay;
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
#define RETURN_GPIO 19
#define SELECT_GPIO 20
#define NEXT_GPIO 21
#define GPIO_DEBOUCE_MS 15
// OLED resolution selector
#define OLED_Y_RES_32_SELECT_GPIO 17
#define OLED_Y_RES_64_SELECT_GPIO 18
// I2C config
#define I2C_WIRE1_SDA_GPIO 14
#define I2C_WIRE1_SCL_GPIO 15
// RGB_LED pins
#define LED_GPIO_R 13
#define LED_GPIO_G 12
#define LED_GPIO_B 11
#define LED_SINK 10
RgbLED led;
// Buzzer
#define BUZZER_GPIO 9
PicoBuzzer buzzer;
// Timer Pools
alarm_pool_t *alarm_pool_primary;
alarm_pool_t *alarm_pool_secondary;
alarm_pool_t *alarm_pool_destroyable;

absolute_time_t DeBounce;
alarm_id_t hold_callbackID;

struct repeating_timer timer;

// put function declarations here:
bool noUpdate = false;
bool printMsg = false;
bool recordedRead = true;
String output = String();
queue_t pendingWorkQueue;
queue_t interruptQueue;
int pinConfig(void);
int USBHidSetup();
int serialSetup();
int setupInitialAlarmPool();
int OLEDSetup();
int SerialDisplaySetup(SerialUART *uart);
int LCDisplaySetup();
int addPrograms();
int passDataToPrograms();
int threadSafeQueues();
int attachInterrupts();
int64_t hold_callback(alarm_id_t id, void *user_data);
int64_t tone_callback(alarm_id_t id, void *user_data);
void gpio_callback(uint gpio, uint32_t events);

void hid_report_callback(uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize);
uint8_t keycode[6] = {0};
uint8_t count = 0;

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
    digitalWrite(LED_GPIO_G, !digitalRead(LED_GPIO_G));
  }
}
Adafruit_USBD_HID usb_hid;
uint8_t const conv_table[128][2] = {HID_ASCII_TO_KEYCODE};
void setup()
{

  USBHidSetup();
  usb_hid.begin();
  serialSetup();
  hold_callbackID = (int32_t)-1;

  pinConfig();
  setupInitialAlarmPool();
  buzzer = PicoBuzzer(BUZZER_GPIO);
  buzzer.set_volume((uint8_t)255);
  buzzer.assign_alarm_pool(alarm_pool_secondary);
  Serial1.printf("buzzerGPIO %u \n", buzzer.get_GPIO());
  Serial1.printf("buzzerVolume %u \n", buzzer.get_volume());
  SongNote note(NOTE_A4, 10000);
  Serial1.printf("note %u \n", note.note);
  Serial1.printf("len %u \n", note.len_us);

  led = RgbLED(LED_GPIO_R, LED_GPIO_G, LED_GPIO_B, LED_SINK);
  led.set_brightness(0.5);
  led.set_color(130, 255, 151);
  // multicore_launch_core1(c1Entry);

  supervisor.assign_alarm_pool(alarm_pool_destroyable);

  threadSafeQueues();
  if ((!digitalRead(OLED_Y_RES_32_SELECT_GPIO) | !digitalRead(OLED_Y_RES_64_SELECT_GPIO)))
  {
    //  LCDisplaySetup();
    Serial1.println("Booting into GUI...");
    OLEDSetup();
  }
  else
  {
    SerialDisplaySetup(&Serial1);
  };

  addPrograms();
  passDataToPrograms();
  attachInterrupts();

  //  scrrenObj.init();
  //  Serial1.println("screen int'd");
  //  scrrenObj.safe_output(message.c_str());
  supervisor.finalize();
  supervisor.startup_finish();
  // usb_hid.begin();
  led.turn_on();
  led.update_output();
  supervisor.run();
  buzzer.play(happy_song);
}

void type(const char *character, int delay_ms = 5)
{

  while (*character != '\0')
  {
    uint8_t keycode[6] = {0};
    uint8_t modifier = 0;
    uint8_t TAB_KEY = 0x09;
    char lookup_target;
    if (TAB_KEY == *(uint8_t *)character)
    {
      keycode[0] = 0x2b;
      // usb_hid.keyboardReport(0, modifier, );
    }
    else
    {
      switch (*character)
      {
      case '@':
        lookup_target = '"';
        break;

      default:
        lookup_target = *character;
        break;
      }
      if (conv_table[lookup_target][0])
      {
        modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
      };
      keycode[0] = conv_table[lookup_target][1];
    };
    usb_hid.keyboardReport(0, modifier, keycode);
    // usb_hid.keyboardPress(0, *character);
    delay(delay_ms);
    usb_hid.keyboardRelease(0);
    delay(delay_ms);
    character++;
  };
}

byte value = 0;
PendingWork queueOutput;
String text = "Hello";
void loop()
{

  // Serial1.println("blocking \n");
  queue_remove_blocking(&pendingWorkQueue, &queueOutput);
  switch (queueOutput.TYPE)
  {
  case BUTTON:

    buzzer.play(button_press);
    switch (queueOutput.PENDING_OBJECT_ID)
    {
    case HOME_BUTTON:
      led.turn_off();
      led.update_output();
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
      led.set_state(0, 0, 255, 0.75);
      led.turn_on();
      led.update_output();
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
  pinMode(OLED_Y_RES_32_SELECT_GPIO, INPUT_PULLUP);
  pinMode(OLED_Y_RES_64_SELECT_GPIO, INPUT_PULLUP);
  pinMode(LED_GPIO_R, OUTPUT);
  pinMode(LED_GPIO_G, OUTPUT);
  pinMode(LED_GPIO_B, OUTPUT);
  pinMode(LED_SINK, INPUT_PULLDOWN);
  pinMode(BUZZER_GPIO, OUTPUT);
  return 0;
};

int USBHidSetup()
{
  usb_hid = Adafruit_USBD_HID(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);
  usb_hid.setReportCallback(NULL, hid_report_callback);
  return 0;
};

int serialSetup()
{
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial1.println("Serial1");
  Serial1.println("-------------------------------");
  return 0;
};

int OLEDSetup()
{
  u8_t y_res = 64;
  if (!digitalRead(OLED_Y_RES_32_SELECT_GPIO))
  {
    y_res = 32;
  }
  else if (!digitalRead(OLED_Y_RES_64_SELECT_GPIO))
  {
    y_res = 64;
  };
  Wire1.setSDA(I2C_WIRE1_SDA_GPIO);
  Wire1.setSCL(I2C_WIRE1_SCL_GPIO);

  Wire1.setClock(400000UL);
  Wire1.begin();
  // Adafruit_SSD1306 wire1Screen = Adafruit_SSD1306(128, 64, &Wire1, -1, 400000UL, 400000UL);
  //  SSD1306_SWITCHCAPVCC
  //  SSD1306_EXTERNALVCC
  // wire1Screen.begin(SSD1306_SWITCHCAPVCC, 0x3C, true, false);
  // wire1Screen.display();
  supervisor.splashScreenDuringStartup(false);
  supervisor.startup_begin();
  // scrrenObj = OLEDUIDisplay();
  // scrrenObj = OLEDUIDisplay(&Wire, 0x3C, 128, 64, 0);
  scrrenObj = OLEDUIDisplay(&Wire1, 0x3C, 128, y_res, 0);
  supervisor.set_UIDisplay(&scrrenObj);
  return 0;
}

/*
int OLEDSetup()
{
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.setClock(100000UL);
  Serial1.println("wire1begin");
  Wire.begin();
  Serial1.println("newobj");
  scrrenObj = OLEDUIDisplay(&Wire, 0x3C, 128, 64, 0);
  Serial1.println("init");
  scrrenObj.init();
  Serial1.println("delay");
  delay(5000);
  supervisor.splashScreenDuringStartup(false);
  supervisor.startup_begin();
  supervisor.set_UIDisplay(&scrrenObj);
  Serial1.println("return");
  return 0;
}
*/

int SerialDisplaySetup(SerialUART *uart)
{
  supervisor.splashScreenDuringStartup(false);
  supervisor.startup_begin();
  serialDisplay = DEBUGSerialUIDisplay(uart);
  supervisor.set_UIDisplay(&serialDisplay);
  return 0;
}

int LCDisplaySetup()
{
  supervisor.splashScreenDuringStartup(false);
  supervisor.startup_begin();
  LCScreen = LCUIDisplay();
  supervisor.set_UIDisplay(&LCScreen);
  return 0;
}

int attachInterrupts()
{
  gpio_set_irq_enabled_with_callback(RETURN_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(SELECT_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(NEXT_GPIO, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
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
  Hello
#endif
}

int addPrograms()
{
  supervisor.add_program(new TimerProgram, sizeof(TimerProgram));
  supervisor.add_program(new AutoLoginProgram, sizeof(AutoLoginProgram));
  return 0;
};

std::vector<AutoLoginProgram::AccountDetails> accoutsVector;
AutoLoginProgram::AutoLoginProgramData autLogin;
int passDataToPrograms()
{

  // AUTOLOGIN
  AutoLoginProgram temp;
  // printf(exampleAccount.displayName.c_str());
  printf("\n");

  // accoutsVector.emplace_back(exampleAccount);
  accoutsVector.emplace_back(AutoLoginProgram::AccountDetails("Acc1", "Username1", "Password1"));
  accoutsVector.emplace_back(AutoLoginProgram::AccountDetails("Acc2", "Username2", "Password2"));
  autLogin.accouts = accoutsVector;
  autLogin.functPtr = type;
  printf("passed accounts verctor len: %i \n", accoutsVector.size());
  std::string id = temp.getID();
  supervisor.passDataToProgramID(id, &autLogin);

  return 0;
}
