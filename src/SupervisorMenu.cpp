#include "SupervisorMenu.h"
char name[] = "OS_MENU";
SupervisorMenu::SupervisorMenu(DesktopicoProgram* programs): DesktopicoProgram(name) {
  typedef void(*x)();
  clear_value();
  returnValue.data = &message;
  UIButton RET = returnValue.buttonSet.RETURN.second;
  std::string RET_id = returnValue.buttonSet.RETURN.first;
  UIButton SEL = returnValue.buttonSet.SELECT.second;
  std::string SEL_id = returnValue.buttonSet.SELECT.first;
  RET.setCallback((x)&SupervisorMenu::set_value);
  SEL.setCallback((x)&SupervisorMenu::clear_value);
  DeclaredButton newReturn = DeclaredButton(RET_id, RET);
  DeclaredButton newSelect = DeclaredButton(SEL_id, SEL);
  returnValue.buttonSet.RETURN.swap(newReturn);
  returnValue.buttonSet.SELECT.swap(newSelect);
}


SupervisorMenu::~SupervisorMenu(){
  
}

ProgramReturn* SupervisorMenu::run(UIButtonSet* availableButtons){

  return &returnValue;
  
  
}

void SupervisorMenu::set_value(){
  message = std::string("HELLO");
}

void SupervisorMenu::clear_value(){
  message = std::string("");
}


