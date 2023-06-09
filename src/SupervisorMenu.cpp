#include "SupervisorMenu.h"
const std::string ID = std::string("OS_MENU");
const std::string displayable = std::string("Main menu");
SupervisorMenu::SupervisorMenu() : DesktopicoProgram(ID, displayable)
{

  current_index = -1;

  FORMAT_PRIORITY = {LIST_OPTIONS_INDEXED, LIST_OPTIONS_SIMPLE, HEADING_LIST};
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

  return_data = LIST_OPTIONS_SIMPLE_STRUCT((int *)&current_index, listOfPrograms);
  returnValue.data = &return_data;
}

SupervisorMenu::~SupervisorMenu()
{
}

void SupervisorMenu::init()
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
  processPassedDataToProgramList();
  if (get_number_of_available_programs() > 1)
  {
    set_current_index(1);
  }
}

int SupervisorMenu::get_number_of_available_programs()
{
  return listOfPrograms.size();
}

bool SupervisorMenu::set_current_index(int index)
{
  if ((index < 0) || (index >= get_number_of_available_programs()))
  {
    return true;
  }
  else
  {
    current_index = index;
    return false;
  };
}

int *SupervisorMenu::set_supervisor_funct(std::function<void()> *target)
{
  supervisor_target = *target;
  supervisor_target();
  return (&current_index);
}

ProgramReturn *SupervisorMenu::run(int *availableButtons)
{
  // previous();

  return_data = LIST_OPTIONS_SIMPLE_STRUCT((int *)&current_index, listOfPrograms);
  returnValue.data = &return_data;
  return &returnValue;
}

void SupervisorMenu::previous()
{
  current_index--;
  // message = intToString(current_index);
  // ProgramDefinedButtons.NEXT.setID("X");
  // message = std::string("");
}

void SupervisorMenu::select()
{
  if (&supervisor_target != nullptr)
  {
    supervisor_target();
  }
  // message = std::string("SELECTED: ");
  // message.append(intToString(current_index));
}

void SupervisorMenu::next()
{
  current_index++;
  // message = intToString(current_index);
}

void SupervisorMenu::processPassedDataToProgramList()
{

  if ((hasDataBeenPassed()) && listOfPrograms.size() == 0)
  {
    std::vector<DesktopicoProgram *> LOL = *(std::vector<DesktopicoProgram *> *)getDataPtr();
    auto itter = LOL.begin();
    auto endOfVector = LOL.end();
    while (itter != endOfVector)
    {
      listOfPrograms.emplace_back((*itter)->getDisplayableName());
      itter++;
    };
  };
}

std::string SupervisorMenu::intToString(int value)
{
  return std::to_string(value);
}
