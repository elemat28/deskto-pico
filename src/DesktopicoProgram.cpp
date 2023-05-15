#include "DesktopicoProgram.h"

bool DesktopicoProgram::hasDataBeenPassed()
{
  return _hasDataBeenPassed;
}

void *DesktopicoProgram::getDataPtr()
{
  return _dataObject;
}

DesktopicoProgram::DesktopicoProgram(std::string program_ID, std::string displayName) : static_ID(program_ID), displayAs(displayName)
{

  _hasDataBeenPassed = false;
  returnValue = ProgramReturn();
}

void DesktopicoProgram::pass_buttons(UIButton buttons[])
{
  _buttons = buttons;
}

void DesktopicoProgram::pass_data(void *dataObject)
{
  _dataObject = dataObject;
  _hasDataBeenPassed = true;
}
int DesktopicoProgram::getNumOfCharsInID()
{
  return _finalIDCharIndex;
}

std::string DesktopicoProgram::getID()
{
  return static_ID;
};

std::string DesktopicoProgram::getDisplayableName()
{
  if (displayAs.empty())
  {
    return static_ID;
  }
  else
  {
    return displayAs;
  }
}


