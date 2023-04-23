#include "DeskopicoProgram.h"

bool DeskopicoProgram::hasDataBeenPassed(){
  return _hasDataBeenPassed;
}

void* DeskopicoProgram::getDataPtr(){
  return _dataObject;
}

DeskopicoProgram::DeskopicoProgram(char program_name[]){
  _finalIDCharIndex = 0;
  _hasDataBeenPassed = false;
  while((_finalIDCharIndex < MAXPICOPROGRAMIDCHARS) && (program_name[_finalIDCharIndex] != '\0')){
      _programID[_finalIDCharIndex] = program_name[_finalIDCharIndex];
      _finalIDCharIndex++;
    };
    _programID[_finalIDCharIndex] = '\0';
}
   
void DeskopicoProgram::pass_buttons(UIButton buttons[]){
  _buttons = buttons;
}

void DeskopicoProgram::pass_data(void* dataObject){
  _dataObject = dataObject;
  _hasDataBeenPassed = true;
}
int DeskopicoProgram::getNumOfCharsInID(){
  return _finalIDCharIndex;
}
