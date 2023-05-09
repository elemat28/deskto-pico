#include "Supervisor.h"

Supervisor::Supervisor() : OS_MENU(), SYS_INFO(BasicRequiredInfo("DESKTO-PICO", 1.0, "elemat28"))
{
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

void Supervisor::run()
{

  if (_pendingButton)
  {
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
  if (_auto_refresh_live)
  {
    if (time_reached(_auto_refresh))
    {
      _pendingScreenRefresh = true;
      _auto_refresh_live = false;
    }
  }
  else
  {
    if (_program_refresh_ms < 0)
    {
    }
    else if (_program_refresh_ms == 0)
    {
      _pendingScreenRefresh = true;
    }
    else if (_program_refresh_ms > 0)
    {
      _auto_refresh_live = true;
      _auto_refresh = make_timeout_time_ms(_program_refresh_ms);
    };
  };
  if (_pendingScreenRefresh)
  {
    _pendingScreenRefresh = false;
    _hasTargetOutputChanged = false;
    hardwareDisplay->output_auto(returnedOutput);
  };
  // hardwareDisplay->output_auto(returnedOutput);
}

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
    _pendingScreenRefresh = false;
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
  _currentRunTarget = &OS_MENU;
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
