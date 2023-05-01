#include "AboutSystemInfo.h"
std::string AboutSystemInfo::ID("OS_INFO");
std::string AboutSystemInfo::displayAs("System INFO");
AboutSystemInfo::AboutSystemInfo(BasicRequiredInfo baseInfo):DesktopicoProgram(ID) {
    
}
 
void AboutSystemInfo::init(){
  return_button_funct = [this](){ previous(); };
  select_button_funct = [this](){ select(); };
  next_button_funct = [this](){ next(); };
  ProgramDefinedButtons.RETURN.setCallbackFunction(return_button_funct);
  ProgramDefinedButtons.SELECT.setCallbackFunction(select_button_funct);
  ProgramDefinedButtons.NEXT.setCallbackFunction(next_button_funct);
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;
}


ProgramReturn* AboutSystemInfo::run(UIButtonSet* availableButtons){
  //previous();
  
  returnValue.data = &systemDetails;
  return &returnValue;
  
}

void AboutSystemInfo::select(){
    
}

void AboutSystemInfo::previous(){

}

void AboutSystemInfo::next(){
    
}


