#include "DeskopicoProgram.h"

bool DesktopicoProgram::hasDataBeenPassed(){
  return _hasDataBeenPassed;
}

void* DesktopicoProgram::getDataPtr(){
  return _dataObject;
}

DesktopicoProgram::DesktopicoProgram(char program_name[]){
  _finalIDCharIndex = 0;
  _hasDataBeenPassed = false;
  while((_finalIDCharIndex < MAXPICOPROGRAMIDCHARS) && (program_name[_finalIDCharIndex] != '\0')){
      _programID[_finalIDCharIndex] = program_name[_finalIDCharIndex];
      _finalIDCharIndex++;
    };
    _programID[_finalIDCharIndex] = '\0';
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
