#include "SupervisorMenu.h"
std::string SupervisorMenu::ID("OS_MENU");
SupervisorMenu::SupervisorMenu(): DesktopicoProgram(ID) {

  listOfPrograms.emplace_back("OPTION_1");
  listOfPrograms.emplace_back("OPTION2");
  listOfPrograms.emplace_back("OPTION_3X");
  listOfPrograms.emplace_back("OPTION_4XX");
  listOfPrograms.emplace_back("OPTION_5XXX");
  listOfPrograms.emplace_back("OPTION_6XXXX");
  listOfPrograms.emplace_back("OPTION_7XXXXX");
  current_index = 0;
  
  FORMAT_PRIORITY = {LIST_OPTIONS_INDEXED, LIST_OPTIONS_SIMPLE, HEADING_LIST};
  ProgramDefinedButtons = UIButtonSet();
  //clear_value();
  returnValue.PROGRAM_ID = ID;

  returnValue.buttonSet = &ProgramDefinedButtons;
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;

  return_button_funct = [this](){ previous(); };
  select_button_funct = [this](){ select(); };
  next_button_funct = [this](){ next(); };
  
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
  
  return_data = LIST_OPTIONS_SIMPLE_STRUCT((int*)&current_index, listOfPrograms);
  returnValue.data = &return_data;

  
 
}


SupervisorMenu::~SupervisorMenu(){
  
}

void SupervisorMenu::init(){
  return_button_funct = [this](){ previous(); };
  select_button_funct = [this](){ select(); };
  next_button_funct = [this](){ next(); };
  ProgramDefinedButtons.RETURN.setCallbackFunction(return_button_funct);
  ProgramDefinedButtons.SELECT.setCallbackFunction(select_button_funct);
  ProgramDefinedButtons.NEXT.setCallbackFunction(next_button_funct);
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;
}


ProgramReturn* SupervisorMenu::run(UIButtonSet* availableButtons){
  //previous();
  
  return_data = LIST_OPTIONS_SIMPLE_STRUCT((int*)&current_index, listOfPrograms);
  returnValue.data = &return_data;
  return &returnValue;
  
  
}

void SupervisorMenu::previous(){
  current_index--;
  //message = intToString(current_index);
  //ProgramDefinedButtons.NEXT.setID("X");
  //message = std::string("");
}


void SupervisorMenu::select(){
  //message = std::string("SELECTED: ");
  //message.append(intToString(current_index));

}


void SupervisorMenu::next(){
  current_index++;
  //message = intToString(current_index);

}

std::string SupervisorMenu::intToString(int value){
  return std::to_string(value);
}

