#include "Supervisor.h"




Supervisor::Supervisor(): OS_MENU(), SYS_INFO(BasicRequiredInfo("DESKTO-PICO", 1.0, "elemat28")) {
  _ver = 0.1;
  BasicRequiredInfo info("DESKTO-PICO", 1.0, "elemat28");
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
  std::function<void(void)> HOME_button_funct = std::bind(&Supervisor::_return_to_main_menu, this);
  std::function<void(void)> HOME_button = std::bind(&Supervisor::return_to_menu, this);
 REQUIRED_BUTTONS.RETURN.setCallbackFunction(return_button_funct);
 REQUIRED_BUTTONS.SELECT.setCallbackFunction(select_button_funct);
 REQUIRED_BUTTONS.NEXT.setCallbackFunction(next_button_funct);
 HOME_BUTTON = UIButton("OS_HOME");
 HOME = HOME_BUTTON;
 HOME_BUTTON.setCallbackFunction(HOME_button_funct);
 HOME.setCallbackFunction(HOME_button);
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
  SYS_INFO.init();
  myPrograms.emplace_back(&SYS_INFO);
  OS_MENU.pass_data(&myPrograms);
  target = [this](){ change_run_target(); };
  supervisorMenuTargetIndex = OS_MENU.set_supervisor_funct(&target);
  finalized = true;
  }
}

void Supervisor::prep_target(){
  _currentRunTarget->init();
  returnedOutput = _currentRunTarget->run((UIButtonSet*)nullptr);
  hardwareDisplay->output_auto(returnedOutput);
  hardwareDisplay->clear();
  _pendingScreenRefresh = true;
}


void Supervisor::change_run_target(){
  if(supervisorMenuTargetIndex!= nullptr){
    logMessage = std::to_string(*supervisorMenuTargetIndex);
    _currentRunTarget = myPrograms.at(*supervisorMenuTargetIndex);

    prep_target();
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
  prep_target();
  
    
}

void Supervisor::run(){
  
  
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
  returnedOutput = _currentRunTarget->run((UIButtonSet*)nullptr);
  hardwareDisplay->output_auto(returnedOutput);
  _pendingScreenRefresh = false;
  //hardwareDisplay->output_auto(returnedOutput);
  
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

void Supervisor::return_to_menu(){
  _return_to_main_menu();
}

bool Supervisor::peekhasWork(){
  return (_pendingButton || _pendingScreenRefresh);
}

void Supervisor::_trigger_return(){
  _pendingButton = true;
  _pressedIndex = 0;
  //_currentRunTarget->ProgramDefinedButtons.RETURN.trigger_function();
}

void Supervisor::_trigger_select(){
  _pendingButton = true;
  _pressedIndex = 1;
  //_currentRunTarget->ProgramDefinedButtons.SELECT.trigger_function();
}

void Supervisor::_trigger_next(){
  _pendingButton = true;
  _pressedIndex = 2;
 // _return_to_main_menu();
 //_currentRunTarget->ProgramDefinedButtons.NEXT.trigger_function();
}

void Supervisor::_return_to_main_menu(){
  _currentRunTarget = &OS_MENU;
  run();
  //hardwareDisplay->safe_output((const char*)x.c_str());
  //_pendingButton = true;
  //returnedOutput = OS_MENU.run((UIButtonSet*)nullptr);

}

std::string Supervisor::getLogs(){
  return logMessage;
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

