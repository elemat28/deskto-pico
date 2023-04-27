#include "Supervisor.h"




Supervisor::Supervisor(): OS_MENU() {
  _ver = 0.1;
  
  _splashScrenDuringStartup = true;
  temp_numOfPrograms = 0;
  _currentRunTarget = nullptr;
  temp_startupTarget = nullptr;
  temp_arrayOfPrograms = {};
  arrayOfPrograms = {};
  finalized = false;
  temp_hardwareDisplay = nullptr;
  _pendingButton = false;
  _pendingScreenRefresh = false;
  REQUIRED_BUTTONS = UIButtonSet();
  std::function<void(void)> return_button_funct = std::bind(&Supervisor::_trigger_return, this);
  std::function<void(void)> select_button_funct = std::bind(&Supervisor::_trigger_select, this);
  std::function<void(void)> next_button_funct = std::bind(&Supervisor::_trigger_next, this);
 REQUIRED_BUTTONS.RETURN.setCallbackFunction(return_button_funct);
 REQUIRED_BUTTONS.SELECT.setCallbackFunction(select_button_funct);
 REQUIRED_BUTTONS.NEXT.setCallbackFunction(next_button_funct);
}

Supervisor::Supervisor(DesktopicoProgram* programs): Supervisor(){
}

Supervisor::~Supervisor(){

}

int Supervisor::setBaseButtonGPIO(UIButton button, int GPIO){
  return 1;
}

void Supervisor::add_function(DesktopicoProgram* program){
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
  //hardwareDisplay->safe_output((char*)REQUIRED_BUTTONS.NEXT.first.c_str());
  
}

void Supervisor::run(){
  
  returnedOutput = _currentRunTarget->run((UIButtonSet*)nullptr);
  if(_pendingButton){
    _pendingButton = false;
    if(_pressedIndex == 0){
      _currentRunTarget->ProgramDefinedButtons.RETURN.trigger_function();
    }else if(_pressedIndex == 1){
      _currentRunTarget->ProgramDefinedButtons.SELECT.trigger_function();
    }else if(_pressedIndex == 2){
      _currentRunTarget->ProgramDefinedButtons.NEXT.trigger_function();
    }
  }
  //hardwareDisplay->output_auto(returnedOutput);
  hardwareDisplay->safe_output((( std::string*)returnedOutput->data)->c_str());
  
}

bool Supervisor::hasWork(){
  bool checkresult = false;
  if (_pendingButton){
    checkresult = true;
    _pendingButton = false;
  }
  if(_pendingScreenRefresh){
    checkresult = true;
    _pendingScreenRefresh = false;
  }
  return checkresult;
}

bool Supervisor::peekhasWork(){
  return (_pendingButton || _pendingScreenRefresh);
}

void Supervisor::_trigger_return(){
  _pendingButton = true;
  _pressedIndex = 0;
}

void Supervisor::_trigger_select(){
  _pendingButton = true;
  _pressedIndex = 1;
}

void Supervisor::_trigger_next(){
  _pendingButton = true;
  _pressedIndex = 2;
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

