#include "Supervisor.h"
std::vector<SongNote> button_press = {
    SongNote(NOTE_G3, 50),
    // SongNote(NOTE_B3, 100)
};
Supervisor::Supervisor() : OS_MENU(),
                           SYS_INFO(BasicRequiredInfo("DESKTO-PICO", 1.0, "elemat28"))
{
  SHORT_ERROR = {
      SongNote(NOTE_DS3, 150),
      SongNote(NOTE_D3, 50),
      SongNote(NOTE_F3, 50),
      SongNote(NOTE_DS3, 150),
      SongNote(NOTE_D3, 50),
      SongNote(NOTE_F3, 50),
      SongNote(NOTE_DS3, 150),
      SongNote(NOTE_D3, 50),
      SongNote(NOTE_F3, 50)};

  _ver = 0.1;
  BasicRequiredInfo info("DESKTO-PICO", 1.0, "elemat28");
  _splashScreenDuringStartup = true;
  _splashScreenMinLenMS = 500;
  temp_numOfPrograms = 0;
  _currentRunTarget = nullptr;
  temp_startupTarget = nullptr;
  temp_arrayOfPrograms = {};
  arrayOfPrograms = {};
  finalized = false;
  temp_hardwareDisplay = nullptr;
  _pendingButton = false;
  _pendingScreenRefresh = false;
  _auto_refresh_live = false;
  REQUIRED_BUTTONS = UIButtonSet();
  _refresh_alarm_ID = (int32_t)-1;
  POOL_ID = 0;
  NUM_OF_TIMERS = 62;
  current_notifcation_state = DEFAULT;
  std::function<void(void)> return_button_funct = std::bind(&Supervisor::_trigger_return, this);
  std::function<void(void)> select_button_funct = std::bind(&Supervisor::_trigger_select, this);
  std::function<void(void)> next_button_funct = std::bind(&Supervisor::_trigger_next, this);
  std::function<void(void)> HOME_button_funct = std::bind(&Supervisor::_return_to_main_menu, this);
  std::function<void(void)> HOME_button = std::bind(&Supervisor::return_to_menu, this);
  REQUIRED_BUTTONS.RETURN.setCallbackFunction(return_button_funct);
  REQUIRED_BUTTONS.SELECT.setCallbackFunction(select_button_funct);
  REQUIRED_BUTTONS.NEXT.setCallbackFunction(next_button_funct);
  HOME_BUTTON = UIButton("OS_HOME");
  HOME = HOME_BUTTON;
  HOME_BUTTON.setCallbackFunction(HOME_button_funct);
  HOME.setCallbackFunction(HOME_button);
  /*
   */
}

Supervisor::Supervisor(BasicRequiredInfo INFO) : Supervisor()
{
}

Supervisor::Supervisor(DesktopicoProgram *programs) : Supervisor()
{
}

Supervisor::~Supervisor()
{
}

int Supervisor::setBaseButtonGPIO(UIButton button, int GPIO)
{
  return 1;
}

bool Supervisor::splashScreenDuringStartup()
{
  return _splashScreenDuringStartup;
}

void Supervisor::splashScreenDuringStartup(bool newSettting)
{
  _splashScreenDuringStartup = newSettting;
}

int Supervisor::splashScreen_min_ms()
{
  return _splashScreenMinLenMS;
}

void Supervisor::splashScreen_min_ms(int minimum_ms)
{
  _splashScreenMinLenMS = minimum_ms;
}

void Supervisor::add_program(DesktopicoProgram *program, size_t programSize)
{
  void *allocated_with_malloc = malloc(programSize);
  memcpy(allocated_with_malloc, program, programSize);
  DesktopicoProgram *ptr = (DesktopicoProgram *)allocated_with_malloc;
  myPrograms.emplace_back(ptr);
}

bool Supervisor::passDataToProgramID(std::string programID, void *data)
{
  bool result = false;
  auto itter = myPrograms.begin();
  while ((!result) && (itter != myPrograms.end()))
  {
    auto deref = *itter;
    if (deref->getID() == programID)
    {
      deref->pass_data(data);
      result = true;
    };
    itter++;
  }

  return result;
}

void Supervisor::set_UIDisplay(UIDisplayHandler *display)
{
  temp_hardwareDisplay = display;
  if (_splashScreenDuringStartup)
  {
    temp_hardwareDisplay->init();
    std::string welcomeMsg = "Starting...";
    temp_hardwareDisplay->safe_output((char *)welcomeMsg.c_str());
  };
}

void Supervisor::set_workQueue(queue_t *queue_ptr)
{
  temp_workQueue = queue_ptr;
}

void Supervisor::set_Buzzer(PicoBuzzer &object)
{
  buzzer = object;
};

void Supervisor::set_RGBLed(RgbLED &object)
{
  notificationLED = object;
};

void Supervisor::set_startup_program(char name[])
{
}

void Supervisor::run_program(char name[])
{
}

void Supervisor::finalize()
{
  if (finalized)
  {
    abort();
  }
  else
  {
    if (temp_startupTarget == nullptr)
    {
      temp_startupTarget = &OS_MENU;
    };
    startupTarget = temp_startupTarget;
    if (_currentRunTarget == nullptr)
    {
      _currentRunTarget = startupTarget;
    };
    if (temp_hardwareDisplay != nullptr)
    {
      temp_hardwareDisplay->init();
    };
    hardwareDisplay = temp_hardwareDisplay;
    SYS_INFO.init();
    myPrograms.emplace(myPrograms.begin(), &SYS_INFO);
    OS_MENU.pass_data(&myPrograms);
    target = [this]()
    { change_run_target(); };
    supervisorMenuTargetIndex = OS_MENU.set_supervisor_funct(&target);
    workQueue = temp_workQueue;

    finalized = true;
  }
}

void Supervisor::prep_target()
{

  _currentRunTarget->init();

  returnedOutput = _currentRunTarget->run(nullptr);
  hardwareDisplay->output_auto(returnedOutput);
  hardwareDisplay->clear();
  _pendingScreenRefresh = true;
}

void Supervisor::change_run_target()
{
  if (supervisorMenuTargetIndex != nullptr)
  {
    if ((-1 < *supervisorMenuTargetIndex) && ((long)*supervisorMenuTargetIndex < (long)myPrograms.size()))
    {
      _currentRunTarget = myPrograms.at(*supervisorMenuTargetIndex);
      printf("change run target ");
      printf(_currentRunTarget->getID().c_str());
      printf("\n");
      prep_target();
    };
  };
}

void Supervisor::startup_begin()
{
  if (_splashScreenDuringStartup)
  {
    _endOfSplashScreen = make_timeout_time_ms(_splashScreenMinLenMS);
  };
}

void Supervisor::startup_finish()
{
  if (!finalized)
  {
    abort();
  }
  prep_target();
  if (_splashScreenDuringStartup)
  {
    sleep_until(_endOfSplashScreen);
  };
}

bool repeatingRefresh(repeating_timer *rt)
{
  // RefreshTimerData *passed_data = (RefreshTimerData *)rt->user_data;
  //*(passed_data->alarmID) = rt->alarm_id;
  // *(passed_data->screenRefreshPending) = true;
  return true;
};

int64_t supervisor_alarm_callback(alarm_id_t id, void *user_data)
{
  PendingWork newStruct = PendingWork(REFRESH, (uint)id);
  queue_t *ptr = (queue_t *)user_data;
  queue_try_add(ptr, &newStruct);
  return 0;
};

void Supervisor::run()
{

  bool justSet = false;
  if (_pendingButton)
  {
    blip_info();
    _pendingButton = false;
    _pendingScreenRefresh = true;
    if (_pressedIndex == 0)
    {
      _currentRunTarget->ProgramDefinedButtons.RETURN.trigger_function();
    }
    else if (_pressedIndex == 1)
    {
      _currentRunTarget->ProgramDefinedButtons.SELECT.trigger_function();
    }
    else if (_pressedIndex == 2)
    {
      _currentRunTarget->ProgramDefinedButtons.NEXT.trigger_function();
    }
  }

  returnedOutput = _currentRunTarget->run((int *)&_program_refresh_ms);

  if (_pendingScreenRefresh)
  {
    hardwareDisplay->output_auto(returnedOutput);
    _pendingScreenRefresh = false;
    if (_refresh_alarm_ID > 0)
    {
      if (_refresh_alarm_ID >= NUM_OF_TIMERS)
      {
        _refresh_alarm_ID = 0;
        printf("RESETTING ALARM POOL... \n");
        destroy_and_recreate_alarm_pool();
      }
      else
      {
        alarm_pool_cancel_alarm(alarmPool_ptr, _refresh_alarm_ID);
        if (_refresh_alarm_ID % 10 == 0)
        {
          printf("Alarm id: %li \n", _refresh_alarm_ID);
        }

        _refresh_alarm_ID = 0;
      }
    }
  };

  if (_refresh_alarm_ID <= (int32_t)0)
  {
    if (returnedOutput->refresh_freq_ms > -1)
    {
      // data = RefreshTimerData(alarmPool_ptr, &_refresh_alarm_ID, &_pendingScreenRefresh);
      _refresh_alarm_ID = alarm_pool_add_alarm_in_ms(alarmPool_ptr, returnedOutput->refresh_freq_ms, supervisor_alarm_callback, workQueue, false);
      // printf("val %i \n", _refresh_alarm_ID);
      justSet = true;
      _pendingScreenRefresh = true;
      // alarm_pool_add_repeating_timer_ms(alarm_pool, returnedOutput->refresh_freq_ms, repeatingRefresh, &data, &repeatingRefreshTimer);
    };

    // hardwareDisplay->output_auto(returnedOutput);
  }
  else
  {
    _refresh_alarm_ID =
        _pendingScreenRefresh = true;
  };

  if (_pendingScreenRefresh)
  {
    if (!justSet && _refresh_alarm_ID > (int32_t)0)
    {
      alarm_pool_cancel_alarm(alarmPool_ptr, _refresh_alarm_ID);
      _refresh_alarm_ID = 0;
    };
    _pendingScreenRefresh = false;
    if (returnedOutput->formatOfData == U_DEF)
    {
      hardwareDisplay->output_auto(returnedOutput);
      returnedOutput = _currentRunTarget->run((int *)&_program_refresh_ms);
    };
    hardwareDisplay->output_auto(returnedOutput);
  };
};

void Supervisor::run(bool forceRefresh)
{
  _pendingScreenRefresh = forceRefresh;
};

bool Supervisor::hasWork()
{
  bool checkresult = false;
  if (_pendingButton)
  {
    checkresult = true;
    _pendingButton = false;
  }
  if (_pendingScreenRefresh)
  {
    checkresult = true;
  }
  if (_auto_refresh_live)
  {
    if (time_reached(_auto_refresh))
    {
      checkresult = true;
    }
  }

  return checkresult;
}

void Supervisor::return_to_menu()
{
  _return_to_main_menu();
}

bool Supervisor::peekhasWork()
{
  return (_pendingButton || _pendingScreenRefresh);
}

void Supervisor::_trigger_return()
{
  _pendingButton = true;
  _pressedIndex = 0;
  //_currentRunTarget->ProgramDefinedButtons.RETURN.trigger_function();
}

void Supervisor::_trigger_select()
{
  _pendingButton = true;
  _pressedIndex = 1;
  //_currentRunTarget->ProgramDefinedButtons.SELECT.trigger_function();
}

void Supervisor::_trigger_next()
{
  _pendingButton = true;
  _pressedIndex = 2;
  // _return_to_main_menu();
  //_currentRunTarget->ProgramDefinedButtons.NEXT.trigger_function();
}

void Supervisor::_return_to_main_menu()
{

  destroy_and_recreate_alarm_pool();
  state_default();
  _currentRunTarget = &OS_MENU;
  _currentRunTarget->init();
  run(true);
  _pendingScreenRefresh = true;
  run();
  // hardwareDisplay->safe_output((const char*)x.c_str());
  //_pendingButton = true;
  // returnedOutput = OS_MENU.run((UIButtonSet*)nullptr);
}

std::string Supervisor::getLogs()
{
  return logMessage;
}

void Supervisor::create_alarm_pool()
{
  alarmPool_ptr = alarm_pool_create(POOL_ID, NUM_OF_TIMERS);
}

void Supervisor::assign_alarm_pool(alarm_pool_t *pass_ptr)
{
  alarmPool_ptr = pass_ptr;
}

void Supervisor::destroy_and_recreate_alarm_pool()
{
  alarm_pool_destroy(alarmPool_ptr);
  create_alarm_pool();
}

int Supervisor::debugFunc()
{
  return OS_MENU.getNumOfCharsInID();
}

int Supervisor::debugFunc(void *data)
{
  OS_MENU.pass_data(data);
  if (OS_MENU.hasDataBeenPassed())
  {
    return 1;
  }
  return 0;
}

int Supervisor::_blip_info()
{
  if (&buzzer != nullptr)
  {
    buzzer.play(button_press);
  }
  return 0;
};
int Supervisor::_blip_warn() { return 0; };
int Supervisor::_blip_error() { return 0; };
int Supervisor::_blip_success() { return 0; };
int Supervisor::_short_error()
{
  if (&notificationLED != nullptr)
  {
    notificationLED.set_state(255, 0, 0, 1);
    notificationLED.turn_on();
    notificationLED.update_output();
  }
  else
  {
    Serial1.println("notificationLED null!");
  };
  if (&buzzer != nullptr)
  {
    buzzer.play(SHORT_ERROR);
  }
  return 0;
};
int Supervisor::_short_success() { return 0; };
int Supervisor::_state_default()
{
  if (&notificationLED != nullptr)
  {
    notificationLED.turn_off();
    notificationLED.update_output();
  };
  return 0;
};
int Supervisor::_state_alert()
{
  if (&notificationLED != nullptr)
  {
    notificationLED.set_state(255, 255, 0, 1);
    notificationLED.turn_on();
    notificationLED.update_output();
  }
  else
  {
    Serial1.println("notificationLED null!");
  };
  return 0;
};
int Supervisor::_state_alarm() { return 0; };
int Supervisor::_state_waiting() { return 0; };
int Supervisor::_await_info(uint optional_timeout) { return 0; };
int Supervisor::_await_warn(uint optional_timeout) { return 0; };
int Supervisor::_await_error(uint optional_timeout) { return 0; };
int Supervisor::_await_choice(uint optional_timeout) { return 0; };
