#include "AutoLoginProgram.h"
const std::string ID = std::string("AUTO_KEYINPUT");
const std::string displayable = std::string("Auto Login");
AutoLoginProgram::AutoLoginProgram() : DesktopicoProgram(ID, displayable)
{

  current_screen = ACCOUNTSELECT;
  FORMAT_PRIORITY = {LIST_OPTIONS_SIMPLE};
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
}

AutoLoginProgram::~AutoLoginProgram()
{
}

void AutoLoginProgram::init()
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
  ACCOUNTSELECT_return = returnValue;
  OUTPUTSELECTION_return = returnValue;
  processPassedData();
  current_screen = ACCOUNTSELECT;
}

ProgramReturn *AutoLoginProgram::run(int *refresh_ms)
{
  switch (current_screen)
  {
  case ACCOUNTSELECT:
    ACCOUNTSELECT_configure();
    return &ACCOUNTSELECT_return;
    break;
  case OUTPUTSELECTION:
    OUTPUTSELECTION_configure();
    return &OUTPUTSELECTION_return;
    break;
  default:
    return &returnValue;
    break;
  };
}

void AutoLoginProgram::previous()
{
}

void AutoLoginProgram::select()
{
}

void AutoLoginProgram::next()
{
}

void AutoLoginProgram::processPassedData()
{

  if (hasDataBeenPassed())
  {
    std::vector<DesktopicoProgram *> LOL = *(std::vector<DesktopicoProgram *> *)getDataPtr();
    auto itter = LOL.begin();
    auto endOfVector = LOL.end();
    while (itter != endOfVector)
    {

      itter++;
    }
  }
}

void AutoLoginProgram::ACCOUNTSELECT_configure()
{
  current_screen = ACCOUNTSELECT;
  accountselect_return_data = LIST_OPTIONS_SIMPLE_STRUCT();
  ProgramDefinedButtons.RETURN.setCallbackFunction([this]()
                                                   { current_index--; });
  ProgramDefinedButtons.SELECT.setCallbackFunction([this]() {});
  ProgramDefinedButtons.NEXT.setCallbackFunction([this]()
                                                 { current_index++; });

  ACCOUNTSELECT_return.data = &accountselect_return_data;
}

void AutoLoginProgram::OUTPUTSELECTION_configure()
{
  current_screen = OUTPUTSELECTION;

  outputselection_return_data = OPTION_BUTTONS_STRUCT();
  outputselection_return_data.buttons.RETURN.setDisplayAs("BCK");
  outputselection_return_data.buttons.SELECT.setDisplayAs("PAUSE");
  outputselection_return_data.buttons.NEXT.setDisplayAs("RST");
  ProgramDefinedButtons.RETURN.setCallbackFunction([this]() {});
  ProgramDefinedButtons.NEXT.setCallbackFunction([this]() {});
  OUTPUTSELECTION_return.refresh_freq_ms = -1;
  OUTPUTSELECTION_return.data = &outputselection_return_data;
}
