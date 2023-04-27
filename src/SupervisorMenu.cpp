#include "SupervisorMenu.h"
std::string SupervisorMenu::ID("OS_MENU");
SupervisorMenu::SupervisorMenu(): DesktopicoProgram(ID) {
  clicks = 0;
  FORMAT_PRIORITY = {LIST_OPTIONS_INDEXED, LIST_OPTIONS_SIMPLE, HEADING_LIST};
  ProgramDefinedButtons = UIButtonSet();
  //clear_value();
  returnValue.PROGRAM_ID = ID;
  returnValue.data = &message;
  returnValue.buttonSet = &ProgramDefinedButtons;
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;

  return_button_funct = std::bind(&SupervisorMenu::previous, this);
  select_button_funct = std::bind(&SupervisorMenu::select, this);
  next_button_funct = std::bind(&SupervisorMenu::next, this);
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

 
}


SupervisorMenu::~SupervisorMenu(){
  
}

ProgramReturn* SupervisorMenu::run(UIButtonSet* availableButtons){

  return &returnValue;
  
  
}

void SupervisorMenu::previous(){
  clicks--;
  message = intToString(clicks);
  //ProgramDefinedButtons.NEXT.setID("X");
  //message = std::string("");
}


void SupervisorMenu::select(){
  message = std::string("SELECTED: ");
  message.append(intToString(clicks));

}


void SupervisorMenu::next(){
  clicks++;
  message = intToString(clicks);

}

std::string SupervisorMenu::intToString(int value){
  return std::to_string(value);
}

