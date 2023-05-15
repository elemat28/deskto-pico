#include "AutoLoginProgram.h"
const std::string ID = std::string("AUTO_KEYINPUT");
const std::string displayable = std::string("Auto Login");
AutoLoginProgram::AutoLoginProgram() : DesktopicoProgram(ID, displayable)
{
  accouts.clear();
  current_index = 0;
  current_screen = ACCOUNTSELECT;
  FORMAT_PRIORITY = {LIST_OPTIONS_SIMPLE};
  ACCOUNTSELECT_formats_priority = {LIST_OPTIONS_SIMPLE};
  OUTPUTSELECTION_formats_priority = {OPTION_BUTTONS};
  ProgramDefinedButtons = UIButtonSet();
  // clear_value();
  returnValue.PROGRAM_ID = getID();
  returnValue.buttonSet = &ProgramDefinedButtons;
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;
  returnValue.refresh_freq_ms = -1;
  returnValue.formatOfData = U_DEF;
  ACCOUNTSELECT_return = returnValue;
  OUTPUTSELECTION_return = returnValue;
  accountselect_return_data = LIST_OPTIONS_SIMPLE_STRUCT();
  accountselect_return_data.INDEX = &current_index;
  ACCOUNTSELECT_return.data = &accountselect_return_data;
  ACCOUNTSELECT_return.FORMAT_PREFERENCE = &ACCOUNTSELECT_formats_priority;
  OUTPUTSELECTION_return.FORMAT_PREFERENCE = &OUTPUTSELECTION_formats_priority;
  outputselection_return_data = OPTION_BUTTONS_STRUCT();
  OUTPUTSELECTION_return.data = &outputselection_return_data;
  outputSetting = USERNAME;
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
  accountselect_return_data = LIST_OPTIONS_SIMPLE_STRUCT();
  std::vector<std::string> epmtyOptionsVector;
  accountselect_return_data.OPTIONS_VECTOR = epmtyOptionsVector;
  accountselect_return_data.INDEX = &current_index;
  // quickselect_return_data = OPTION_BUTTONS_STRUCT("TIMER LEN[MM:SS]");
  // returnValue.data = &quickselect_return_data;
}

AutoLoginProgram::~AutoLoginProgram()
{
}

void AutoLoginProgram::init()
{

  printf("AutoLoginProgram init \n");
  processPassedData();
  printf("stored accounts verctor len: %i \n", accouts.size());
  current_screen = ACCOUNTSELECT;
}

ProgramReturn *AutoLoginProgram::run(int *refresh_ms)
{
  switch (current_screen)
  {
  case ACCOUNTSELECT:
    ACCOUNTSELECT_configure();
    // printf("ID: %s \n", ACCOUNTSELECT_return.PROGRAM_ID);
    return &ACCOUNTSELECT_return;
    break;
  case OUTPUTSELECTION:
    OUTPUTSELECTION_configure();
    if (OUTPUTSELECTION_return.formatOfData == U_DEF)
    {
      OUTPUTSELECTION_return.refresh_freq_ms = 0;
    };
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
    printf("dataPassed \n");
    auto passedDataPtr = (std::vector<AutoLoginProgram::AccountDetails> *)getDataPtr();
    auto itter = passedDataPtr->begin();
    printf("vector Size %i \n", passedDataPtr->size());
    while (itter != passedDataPtr->end())
    {
      AccountDetails detail = *itter;
      accouts.push_back(detail);
      accountselect_return_data.OPTIONS_VECTOR.emplace_back(detail.displayName);
      // accouts.push_back(itter->);
      //  accountselect_return_data.OPTIONS_VECTOR.push_back(itter->displayName);
      itter++;
    };
  };
}

void AutoLoginProgram::ACCOUNTSELECT_configure()
{
  current_screen = ACCOUNTSELECT;
  accountselect_return_data.INDEX = &current_index;

  ProgramDefinedButtons.RETURN.setCallbackFunction([this]()
                                                   { current_index--; });
  ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                   { OUTPUTSELECTION_configure(); });
  ProgramDefinedButtons.NEXT.setCallbackFunction([this]()
                                                 { current_index++; });

  ACCOUNTSELECT_return.data = &accountselect_return_data;
}
// USBKeyboard keyboard;
void AutoLoginProgram::OUTPUTSELECTION_configure()
{
  current_screen = OUTPUTSELECTION;

  outputselection_return_data.buttons.SELECT.setDisplayAs("GO");
  switch (outputSetting)
  {
  case USERNAME:
    outputselection_return_data.message = std::string("USERNAME");
    break;
  case PASSWORD:
    outputselection_return_data.message = std::string("PASSWORD");
    break;
  case FULL:
    outputselection_return_data.message = std::string("AUTO LOGIN");
    break;
  default:
    break;
  };

  outputselection_return_data.buttons.RETURN.setDisplayAs("BCK");

  outputselection_return_data.buttons.NEXT.setDisplayAs("MODE");
  ProgramDefinedButtons.RETURN.setCallbackFunction([this]()
                                                   { ACCOUNTSELECT_configure(); });
  ProgramDefinedButtons.SELECT.setCallbackFunction([this]()
                                                   { triggerOutput(); });

  ProgramDefinedButtons.NEXT.setCallbackFunction([this]()
                                                 { int local = 1 + outputSetting;
                                                 if(local>2){
                                                  local = 0;
                                                 };
                                                 outputSetting =  (OutputMode)local; });
  OUTPUTSELECTION_return.refresh_freq_ms = -1;
  OUTPUTSELECTION_return.data = &outputselection_return_data;
}

void AutoLoginProgram::triggerOutput()
{
  printf("init Blocking... \n");
  // keyboard.enable_output(true);
}