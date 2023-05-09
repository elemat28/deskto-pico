#include "main.h"
#define ALIVEMSGFREQSECS 2
#define MENUHOLD_MS 1500
#define DEBOUNCE 120
#define MINSPLASHMS 500

#define RETURN_GPIO 9
#define SELECT_GPIO 8
#define NEXT_GPIO 7

struct PinReading
{
  int gpio;
  bool state;
  PinReading(int GPIO)
  {
    gpio = GPIO;
    state = false;
  };
  PinReading(int GPIO, bool STATE)
  {
    gpio = GPIO;
    state = STATE;
  };
};

typedef struct
{
  int gpio;
  bool state;
} queue_entry_t;

queue_t button_queue;
queue_t results_queue;

bool ignoreRelease = false;
volatile bool sleeping = false;

volatile int pollingAlarmID = 0;
volatile bool GPIO_STATE[32] = {false};
std::array<bool, 32> GPIO_ARRAY = {false};
std::array<bool, 32> GPIO_SCAN_ARRAY = {false};
std::vector<std::string> logVector;
volatile bool (*GPIO_STATE_PTR)[32] = &GPIO_STATE;
const std::vector<int> DEFINED_BUTTONS = {RETURN_GPIO, SELECT_GPIO, NEXT_GPIO};
absolute_time_t bounceBuffer;
std::vector<std::pair<int *, bool>> CURRENT_READINGS;

struct AlivePacket
{
  std::string message;
  volatile bool outstandingPrint;
  AlivePacket()
  {
    message = std::string();
    outstandingPrint = false;
  }
} alivePacket;

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
  timer_fired = true;
  ignoreRelease = true;
  alarm_pool_cancel_alarm(alarm_pool_secondary, id);
  homeButtonAlarmID = 0;
  return 0;
};

void addToLogs(std::string message)
{
  logVector.emplace_back(message);
};

void logsToSerial()
{
  if (!logVector.empty())
  {
    auto itter = logVector.begin();
    while (itter != logVector.end())
    {
      std::string to_print;
      to_print = *itter;
      Serial.println(to_print.c_str());
      itter++;
    };
    logVector.clear();
  };
};

int logFunctionResult(const std::string functionMessage, int (*function_ptr)(), bool logToConsole = true)
{
  if (logToConsole)
  {
    std::string scopeMsg = std::string("[SETUP][");
    scopeMsg.append(functionMessage);
    scopeMsg.append("] ");
    std::string logMessage = scopeMsg;
    logMessage.append("Starting...");
    Serial.println(logMessage.c_str());
    int result = function_ptr();
    logMessage = scopeMsg;
    if (result == 0)
    {
      logMessage.append("OK!");
    }
    else
    {
      logMessage.append("ERROR! %i", result);
    }
    Serial.println(logMessage.c_str());
    return result;
  }
  else
  {
    return function_ptr();
  }
};

int pinSetup()
{
  // RGB led
  pinMode(15, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);
  // encoder
  pinMode(12, INPUT_PULLUP); // CLICK
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  // button
  pinMode(9, INPUT_PULLUP); // CLICK
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  // buzzer
  pinMode(6, OUTPUT);
  return 0;
};

int createUIdisplay()
{
  screen = LCUIDisplay();
  return 0;
};

int addDisplayToSupervisor()
{
  uiSupervisor.set_UIDisplay(&screen);
  return 0;
};

int finalizeSupervisor()
{
  uiSupervisor.finalize();
  return 0;
};

int startupSupervisor()
{
  uiSupervisor.startup_finish();
  return 0;
};

int setupInitialAlarmPool()
{
  alarm_pool_primary = alarm_pool_create(2, 16);
  alarm_pool_secondary = alarm_pool_create(3, 32);
  return 0;
}

bool repeatingPrintAliveFunct(repeating_timer *rt)
{
  AlivePacket *dataPtr = (AlivePacket *)(*rt).user_data;

  (*dataPtr).message.clear();
  (*dataPtr).message.append("Runtime[seconds]: ");
  (*dataPtr).message.append(std::to_string((int)(time_us_64() / (1000 * 1000))).c_str());
  (*dataPtr).outstandingPrint = true;

  return true;
};

bool repeatinHWPolling(repeating_timer *rt)
{

  if (digitalRead(RETURN_GPIO) == GPIO_ARRAY[RETURN_GPIO])
  {
    GPIO_ARRAY[RETURN_GPIO] = !GPIO_ARRAY[RETURN_GPIO];
    PinReading change = PinReading(RETURN_GPIO, GPIO_ARRAY[RETURN_GPIO]);
    if (!queue_is_full(&button_queue))
    {
      queue_add_blocking(&button_queue, &change);
    };
  };
  if (digitalRead(SELECT_GPIO) == GPIO_ARRAY[SELECT_GPIO])
  {
    GPIO_ARRAY[SELECT_GPIO] = !GPIO_ARRAY[SELECT_GPIO];
    PinReading change = PinReading(SELECT_GPIO, GPIO_ARRAY[SELECT_GPIO]);
    if (!queue_is_full(&button_queue))
    {
      queue_add_blocking(&button_queue, &change);
    }
  };
  if (digitalRead(NEXT_GPIO) == GPIO_ARRAY[NEXT_GPIO])
  {
    GPIO_ARRAY[NEXT_GPIO] = !GPIO_ARRAY[NEXT_GPIO];
    PinReading change = PinReading(NEXT_GPIO, GPIO_ARRAY[NEXT_GPIO]);
    if (!queue_is_full(&button_queue))
    {
      queue_add_blocking(&button_queue, &change);
    };
  };

  return true;
};

int setupAlivePrintToSerial()
{
  if (createTimeout(alarm_pool_primary, ALIVEMSGFREQSECS * 1000000, repeatingPrintAliveFunct, (void *)&alivePacket, (repeating_timer_t *)&rtInst))
  {
    return 0;
  }
  else
  {
    return 1;
  }
};

int setupPollingRepeater()
{
  if (createTimeout(alarm_pool_secondary, 10000, repeatinHWPolling, (void *)&GPIO_ARRAY, (repeating_timer_t *)&HWPollingTimer))
  {
    return 0;
  }
  else
  {
    return 1;
  }
};

int addPrograms()
{
  uiSupervisor.add_program(new TimerProgram, sizeof(TimerProgram));

  return 0;
};

void setup()
{
  uiSupervisor.splashScreen_min_ms(0);
  uiSupervisor.startup_begin();
  logFunctionResult("UIDisplay instantiate", createUIdisplay);
  logFunctionResult("Add display to supervisor", addDisplayToSupervisor);

  USBSetup();
  GPIO = -1;
  pollingAlarmID = 0;
  printFreq = get_absolute_time();
  alivePacket = AlivePacket();
  holding = false;
  holdCounting = false;
  callbackResult = false;
  timer_fired = false;
  button_pressed = false;
  construct_alarm = false;
  destruct_alarm = false;
  DeBounce = make_timeout_time_ms(DEBOUNCE);
  homeButtonAlarmID = 0;
  alivePacket.outstandingPrint = true;
  alivePacket.message.reserve(64); // without this timer breaks if string has to be resized in the callback
  bounceBuffer = get_absolute_time();
  queue_init(&button_queue, sizeof(PinReading), 2);
  logFunctionResult("Pin Setup", pinSetup);
  logFunctionResult("add Programs", addPrograms);
  logFunctionResult("Finalize Supervisor", finalizeSupervisor);
  logFunctionResult("Initial Alarm Pool setup", setupInitialAlarmPool);
  logFunctionResult("Repeating ALIVE timer activation", setupAlivePrintToSerial);
  logFunctionResult("Repeating GPIO Polling activation", setupPollingRepeater);
  DeBounce = make_timeout_time_ms(DEBOUNCE);
  ignoreRelease = false;
  uiSupervisor.startup_finish();
};

void loop()
{

  if (timer_fired)
  {
    addToLogs("LONG PRESS!");
    timer_fired = false;
    addToLogs("Returning home...");
    uiSupervisor.HOME.trigger_function();
  };
  if (alivePacket.outstandingPrint)
  {
    alivePacket.outstandingPrint = false;
    addToLogs(alivePacket.message.c_str());
  };
  if (!queue_is_empty(&button_queue))
  {
    PinReading reading = PinReading(0);
    queue_remove_blocking(&button_queue, &reading);
    switch (reading.gpio)
    {
    case RETURN_GPIO:
      if (reading.state == LOW)
      {
        if (ignoreRelease)
        {
          ignoreRelease = false;
        }
        else
        {
          addToLogs("RETURN RELEASED");
          uiSupervisor.REQUIRED_BUTTONS.RETURN.trigger_function();
        };
        if (homeButtonAlarmID > 0)
        {
          alarm_pool_cancel_alarm(alarm_pool_secondary, homeButtonAlarmID);
          homeButtonAlarmID = 0;
        };
      }
      else
      {
        if (homeButtonAlarmID != 0)
        {
          alarm_pool_cancel_alarm(alarm_pool_secondary, homeButtonAlarmID);
          homeButtonAlarmID = 0;
        };
        homeButtonAlarmID = createTimeout_single(alarm_pool_secondary, 1000, alarm_callback, (void *)&holding, true);
      }
      break;

    case SELECT_GPIO:
      if (reading.state == HIGH)
      {
        addToLogs("SELECT PRESSED");
        uiSupervisor.REQUIRED_BUTTONS.SELECT.trigger_function();
      };
      break;

    case NEXT_GPIO:
      if (reading.state == HIGH)
      {
        addToLogs("NEXT PRESSED");
        uiSupervisor.REQUIRED_BUTTONS.NEXT.trigger_function();
      };
      break;

    default:
      break;
    }
  };

  uiSupervisor.run();
  logsToSerial();
  while ((logVector.empty() && !alivePacket.outstandingPrint && queue_is_empty(&button_queue) && !timer_fired && !uiSupervisor.hasWork()))
  {
    sleep_ms(20);
  };
};
