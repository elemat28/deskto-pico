#include "TimerProgram.h"
const std::string ID = std::string("WORK_TIMER");
const std::string displayable = std::string("Timers");
TimerProgram::TimerProgram() : DesktopicoProgram(ID, displayable), timerObject(0)
{

  current_screen = QUICKSELECT;
  FORMAT_PRIORITY = {OPTION_BUTTONS};
  // FORMAT_PRIORITY = {OPTION_BUTTONS, KEY_OPTION_SCROLL};
  ProgramDefinedButtons = UIButtonSet();
  // clear_value();
  returnValue.PROGRAM_ID = getID();

  returnValue.buttonSet = &ProgramDefinedButtons;
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;

  return_button_funct = [this]()
  { previous(); };
  select_button_funct = [this]()
  { select(); };
  next_button_funct = [this]()
  { next(); };

  UIButton temp;

  temp = ProgramDefinedButtons.RETURN;
  temp.setCallbackFunction(return_button_funct);
  ProgramDefinedButtons.RETURN = temp;

  temp = ProgramDefinedButtons.SELECT;
  temp.setCallbackFunction(select_button_funct);
  ProgramDefinedButtons.SELECT = temp;

  temp = ProgramDefinedButtons.NEXT;
  temp.setCallbackFunction(next_button_funct);

  ProgramDefinedButtons.NEXT = temp;

  // quickselect_return_data = OPTION_BUTTONS_STRUCT("TIMER LEN[MM:SS]");
  // returnValue.data = &quickselect_return_data;
  custom_timer_len = 30;
}

TimerProgram::~TimerProgram()
{
}

void TimerProgram::init()
{
  return_button_funct = [this]()
  { previous(); };
  select_button_funct = [this]()
  { select(); };
  next_button_funct = [this]()
  { next(); };
  ProgramDefinedButtons.RETURN.setCallbackFunction(return_button_funct);
  ProgramDefinedButtons.SELECT.setCallbackFunction(select_button_funct);
  ProgramDefinedButtons.NEXT.setCallbackFunction(next_button_funct);
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;
  quickselect_return = returnValue;
  running_return = returnValue;
  custom_return = returnValue;
  custom_timer_len = 30;
  timerObject = Timer(0);
  processPassedData();
  current_screen = QUICKSELECT;
}

ProgramReturn *TimerProgram::run(int *refresh_ms)
{
  switch (current_screen)
  {
  case RUNNING:
    RUNNING_configure();
    return &running_return;
    break;
  case QUICKSELECT:
    QUICKSELECT_configure();
    return &quickselect_return;
    break;
  case CUSTOM:
    CUSTOM_configure();
    if (custom_return.formatOfData == U_DEF)
    {
      custom_return.refresh_freq_ms = 0;
    };
    return &custom_return;
    break;
  default:
    return &returnValue;
    break;
  };
}

void TimerProgram::previous()
{
}

void TimerProgram::select()
{
}

void TimerProgram::next()
{
}

void TimerProgram::processPassedData()
{

  if (hasDataBeenPassed())
  {
    std::vector<DesktopicoProgram *> LOL = *(std::vector<DesktopicoProgram *> *)getDataPtr();
    auto itter = LOL.begin();
    auto endOfVector = LOL.end();
    while (itter != endOfVector)
    {
      listOfPrograms.emplace_back((*itter)->getDisplayableName());
      itter++;
    }
    listOfPrograms.emplace_back("OPTION_1");
  }
}

std::string TimerProgram::intToString(int value)
{
  return std::to_string(value);
}

void TimerProgram::create_timer(int seconds)
{
  timerObject = Timer(seconds);
}

std::string TimerProgram::formatTimerTimeLeftToString(Timer *timerObj)
{
  std::string timeAsString = std::string("");
  TimerTimeLeft returnValue = timerObj->getTimeLeft();
  returnValue = timerObject.getTimeLeft();
  if ((returnValue.minutes > 0) && (returnValue.minutes < 10))
  {
    timeAsString.append("0");
  };
  timeAsString.append(std::to_string(returnValue.minutes));
  timeAsString.append(":");
  if (returnValue.seconds < 10)
  {
    timeAsString.append("0");
  };
  timeAsString.append(std::to_string(returnValue.seconds));
  return timeAsString;
}

void TimerProgram::QUICKSELECT_configure()
{
  current_screen = QUICKSELECT;
  if (supervisor_ptr->get_current_state() != UINotification::OS_NOTIFICATION_STATUS::DEFAULT)
  {
    supervisor_ptr->state_default();
  };
  quickselect_return_data_as_option_buttons = OPTION_BUTTONS_STRUCT("Select minutes");
  quickselect_return_data_as_option_buttons.buttons.RETURN.setDisplayAs("15");
  quickselect_return_data_as_option_buttons.buttons.SELECT.setDisplayAs("30");
  quickselect_return_data_as_option_buttons.buttons.NEXT.setDisplayAs("CSTM");
  ProgramDefinedButtons.RETURN.setCallbackFunction([this]()
                                                   {  create_timer(15*60);
                                                      timerObject.start();
                                                      RUNNING_configure(); });
  ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                   {  create_timer(30*60);
                                                      timerObject.start();
                                                      RUNNING_configure(); });
  ProgramDefinedButtons.NEXT.setCallbackFunction([this]()
                                                 { CUSTOM_configure(); });

  quickselect_return.data = &quickselect_return_data_as_option_buttons;
}

void TimerProgram::RUNNING_configure()
{
  current_screen = RUNNING;
  TimerStatus status = timerObject.getStatus();

  if (timerObject.getTimeLeftAsSeconds() < timerObject.getTimerLenAsSeconds() / 3)
  {
    supervisor_ptr->state_alert();
  }
  else
  {
    supervisor_ptr->state_default();
  };
  running_return_data_as_option_buttons = OPTION_BUTTONS_STRUCT(formatTimerTimeLeftToString(&timerObject));
  running_return_data_as_option_buttons.buttons.RETURN.setDisplayAs("BCK");
  running_return_data_as_option_buttons.buttons.SELECT.setDisplayAs("PAUSE");
  running_return_data_as_option_buttons.buttons.NEXT.setDisplayAs("RST");
  ProgramDefinedButtons.RETURN.setCallbackFunction([this]()
                                                   { QUICKSELECT_configure(); });
  ProgramDefinedButtons.NEXT.setCallbackFunction([this]()
                                                 { timerObject.reset(); });
  running_return.refresh_freq_ms = -1;
  switch (status)
  {
  case NOT_STARTED:
    running_return_data_as_option_buttons.buttons.SELECT.setDisplayAs("START");
    ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                     { timerObject.start(); });
    break;

  case PAUSED:
    running_return_data_as_option_buttons.buttons.SELECT.setDisplayAs("RESUME");
    ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                     { timerObject.start(); });
    break;

  case ACTIVE:
    running_return.refresh_freq_ms = 1000;
    running_return_data_as_option_buttons.buttons.SELECT.setDisplayAs("PAUSE");
    ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                     { timerObject.pause(); });
    break;

  case FINISHED:
    if (supervisor_ptr->get_current_state() == UINotification::OS_NOTIFICATION_STATUS::ALERT)
    {
      supervisor_ptr->short_error();
    };
    running_return_data_as_option_buttons.buttons.SELECT.setDisplayAs(" ");
    break;

  default:
    break;
  };

  running_return.data = &running_return_data_as_option_buttons;
}

void TimerProgram::CUSTOM_configure()
{
  current_screen = CUSTOM;
  custom_return_data_as_option_buttons = OPTION_BUTTONS_STRUCT();
  create_timer(custom_timer_len);
  custom_return_data_as_option_buttons.message = formatTimerTimeLeftToString(&timerObject);
  custom_return_data_as_option_buttons.buttons.RETURN.setDisplayAs("-15S");
  custom_return_data_as_option_buttons.buttons.SELECT.setDisplayAs("OK");
  custom_return_data_as_option_buttons.buttons.NEXT.setDisplayAs("+30S");
  ProgramDefinedButtons.RETURN.setCallbackFunction([this]()
                                                   {  if((custom_timer_len - 15 )> 0){
                                                    custom_timer_len -= 15;
                                                   }; });
  ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                   {  create_timer(custom_timer_len);
                                                      timerObject.start();
                                                      RUNNING_configure(); });
  ProgramDefinedButtons.NEXT.setCallbackFunction([this]()
                                                 { custom_timer_len += 30; });
  custom_return.data = &custom_return_data_as_option_buttons;
}