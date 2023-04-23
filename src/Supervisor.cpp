#include "Supervisor.h"

Supervisor::Supervisor(): OS_MENU(arrayOfPrograms) {
  _ver = 0.1;
  RETURN_BUTTON = UIButton();
  SELECT_BUTTON = UIButton();
  temp_numOfPrograms = 0;
  _currentRunTarget = nullptr;
  temp_startupTarget = nullptr;
  temp_arrayOfPrograms = {};
  arrayOfPrograms = {};
  finalized = false;
  
}

Supervisor::Supervisor(DeskopicoProgram* programs): Supervisor(){
}

Supervisor::~Supervisor(){

}

void Supervisor::add_function(DeskopicoProgram* program){
  if(finalized ||(temp_numOfPrograms >= UISUPRVSRMAXPRGRMS)){
    abort();
  }
  programDeclarations[temp_numOfPrograms] = program;
  temp_numOfPrograms++;

}

void Supervisor::set_startup_program(char name[]){

}

void Supervisor::run_program(char name[]){
  
}

void Supervisor::finalize(){
  if(finalized){
    abort();
  } else {
  if(temp_startupTarget == nullptr){
    temp_startupTarget = &OS_MENU;
  };
  startupTarget = temp_startupTarget;
  if(_currentRunTarget == nullptr){
    _currentRunTarget = startupTarget;
  }
  finalized = true;
  }
}

void Supervisor::run(){
  _currentRunTarget->run();

  
}

int Supervisor::debugFunc(){
  return OS_MENU.getNumOfCharsInID();
}

int Supervisor::debugFunc(void* data){
  OS_MENU.pass_data(data);
  if(OS_MENU.hasDataBeenPassed()){
    return 1;
  }
  return 0;
}

