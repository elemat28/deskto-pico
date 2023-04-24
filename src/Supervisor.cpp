#include "Supervisor.h"




Supervisor::Supervisor(): OS_MENU(arrayOfPrograms) {
  _ver = 0.1;
  _splashScrenDuringStartup = true;
  RETURN_BUTTON = UIButton();
  SELECT_BUTTON = UIButton();
  temp_numOfPrograms = 0;
  _currentRunTarget = nullptr;
  temp_startupTarget = nullptr;
  temp_arrayOfPrograms = {};
  arrayOfPrograms = {};
  finalized = false;
  temp_hardwareDisplay = nullptr;
  
}

Supervisor::Supervisor(DeskopicoProgram* programs): Supervisor(){
}

Supervisor::~Supervisor(){

}

int Supervisor::setBaseButtonGPIO(BASE_BUTTONS baseButton, int GPIO){
  return 1;
}

void Supervisor::add_function(DeskopicoProgram* program){
  if(finalized ||(temp_numOfPrograms >= UISUPRVSRMAXPRGRMS)){
    abort();
  }
  programDeclarations[temp_numOfPrograms] = program;
  temp_numOfPrograms++;

}

void Supervisor::set_UIDisplay(UIDisplayHandler* display){
  temp_hardwareDisplay = display;
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
  };
  if(temp_hardwareDisplay != nullptr){
    temp_hardwareDisplay->init();
  };
  hardwareDisplay = temp_hardwareDisplay;

  finalized = true;
  }
}

void Supervisor::startup(){
  if(!finalized){
    abort();
  }
  if(_splashScrenDuringStartup){
    if(hardwareDisplay != nullptr){
      std::string welcomeMsg = "Starting...";
      welcomeMsg.append(std::to_string(_ver));
      hardwareDisplay->safe_output((char*)welcomeMsg.c_str());
    };
  }
  hardwareDisplay->clear();
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

