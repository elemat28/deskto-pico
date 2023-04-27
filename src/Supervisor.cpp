#include "Supervisor.h"




Supervisor::Supervisor(): OS_MENU(arrayOfPrograms) {
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
  typedef void(*x)(void);
  REQUIRED_BUTTONS = UIButtonSet();
  UIButton temp_SELECT  = REQUIRED_BUTTONS.SELECT.second;
  UIButton temp_RETURN  = REQUIRED_BUTTONS.RETURN.second;
  UIButton temp_NEXT    = REQUIRED_BUTTONS.NEXT.second;
  temp_SELECT .setCallback((x)&Supervisor::_trigger_select);
  temp_RETURN .setCallback((x)&Supervisor::_trigger_return);
  temp_NEXT   .setCallback((x)&Supervisor::_trigger_next);
  std::string SELECT_id = REQUIRED_BUTTONS.SELECT.first;
  std::string RETURN_id = REQUIRED_BUTTONS.RETURN.first;
  std::string NEXT_id   = REQUIRED_BUTTONS.NEXT.first;
  DeclaredButton newSelect = DeclaredButton(SELECT_id, temp_SELECT);
  DeclaredButton newReturn = DeclaredButton(RETURN_id, temp_RETURN);
  DeclaredButton newNext   = DeclaredButton(NEXT_id, temp_NEXT);
  REQUIRED_BUTTONS.SELECT .swap(newSelect);
  REQUIRED_BUTTONS.RETURN .swap(newReturn);
  REQUIRED_BUTTONS.NEXT   .swap(newNext);
}

Supervisor::Supervisor(DesktopicoProgram* programs): Supervisor(){
}

Supervisor::~Supervisor(){

}

int Supervisor::setBaseButtonGPIO(DeclaredButton button, int GPIO){
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
  ProgramReturn* returnval =  _currentRunTarget->run((UIButtonSet*)nullptr);
  if(hasWork()){
    if(_pressedIndex == 0){
      hardwareDisplay->safe_output( (char*)(REQUIRED_BUTTONS.RETURN.first.c_str()));
      //returnval->buttonSet.RETURN.second.trigger();
    }else if(_pressedIndex == 1){
      hardwareDisplay->safe_output( (char*)(REQUIRED_BUTTONS.SELECT.first.c_str()));
      //returnval->buttonSet.SELECT.second.trigger();
    }else if(_pressedIndex == 2){
      hardwareDisplay->safe_output( (char*)(REQUIRED_BUTTONS.NEXT.first.c_str()));
      //returnval->buttonSet.NEXT.second.trigger();
    }
    
    //hardwareDisplay->safe_output(((std::string*)returnval->data)->c_str());
    
  }
  
  
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

