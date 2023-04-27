#include "DeskopicoProgram.h"

bool DesktopicoProgram::hasDataBeenPassed(){
  return _hasDataBeenPassed;
}

void* DesktopicoProgram::getDataPtr(){
  return _dataObject;
}

DesktopicoProgram::DesktopicoProgram(std::string program_ID){
  //ID = program_ID;
  _hasDataBeenPassed = false;

}
   
void DesktopicoProgram::pass_buttons(UIButton buttons[]){
  _buttons = buttons;
}

void DesktopicoProgram::pass_data(void* dataObject){
  _dataObject = dataObject;
  _hasDataBeenPassed = true;
}
int DesktopicoProgram::getNumOfCharsInID(){
  return _finalIDCharIndex;
}


std::string DesktopicoProgram::getID(){
  return ID;
};

