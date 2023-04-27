#include "SupervisorMenu.h"
std::string SupervisorMenu::ID("OS_MENU");
SupervisorMenu::SupervisorMenu(): DesktopicoProgram(ID) {
  FORMAT_PRIORITY = {LIST_OPTIONS_INDEXED, LIST_OPTIONS_SIMPLE, HEADING_LIST};
  ProgramDefinedButtons = UIButtonSet();
  //clear_value();
  returnValue.PROGRAM_ID = ID;
  returnValue.data = &message;
  returnValue.buttonSet = &ProgramDefinedButtons;
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;

  return_button_funct = std::bind(&SupervisorMenu::clear_value, this);
  select_button_funct = std::bind(&SupervisorMenu::set_value, this);
  next_button_funct = std::bind(&SupervisorMenu::set_str, this);
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

void SupervisorMenu::clear_value(){
  message.clear();
  //ProgramDefinedButtons.NEXT.setID("X");
  //message = std::string("");
}


void SupervisorMenu::set_value(){
  message = std::string("ACCEPT!");
}


void SupervisorMenu::set_str(){
  message = std::string("HELLO!");
}


