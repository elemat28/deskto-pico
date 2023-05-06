#include "TimerProgram.h"
const std::string ID = std::string("WORK_TIMER");
const std::string displayable = std::string("Timers");
TimerProgram::TimerProgram(): DesktopicoProgram(ID, displayable) {

  current_screen = QUICKSELECT;
  FORMAT_PRIORITY = {OPTION_BUTTONS};
  //FORMAT_PRIORITY = {OPTION_BUTTONS, KEY_OPTION_SCROLL};
  ProgramDefinedButtons = UIButtonSet();
  //clear_value();
  returnValue.PROGRAM_ID = getID();

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
  
  quickselect_return_data = OPTION_BUTTONS_STRUCT("TIMER LEN[MM:SS]");
  returnValue.data = &quickselect_return_data;

  
 
}


TimerProgram::~TimerProgram(){
  
}

void TimerProgram::init(){
  return_button_funct = [this](){ previous(); };
  select_button_funct = [this](){ select(); };
  next_button_funct = [this](){ next(); };
  ProgramDefinedButtons.RETURN.setCallbackFunction(return_button_funct);
  ProgramDefinedButtons.SELECT.setCallbackFunction(select_button_funct);
  ProgramDefinedButtons.NEXT.setCallbackFunction(next_button_funct);
  returnValue.FORMAT_PREFERENCE = &FORMAT_PRIORITY;
  processPassedData();
  
}




ProgramReturn* TimerProgram::run(UIButtonSet* availableButtons){
  quickselect_return_data = OPTION_BUTTONS_STRUCT("Select minutes");
  quickselect_return_data.buttons.RETURN.setDisplayAs("15");
  quickselect_return_data.buttons.SELECT.setDisplayAs("30");
  quickselect_return_data.buttons.NEXT.setDisplayAs("CSTM");
  returnValue.data = &quickselect_return_data;
  return &returnValue;
}

void TimerProgram::previous(){
  current_index--;
  //message = intToString(current_index);
  //ProgramDefinedButtons.NEXT.setID("X");
  //message = std::string("");
}


void TimerProgram::select(){
  if(&supervisor_target != nullptr){
    supervisor_target();
  }
  //message = std::string("SELECTED: ");
  //message.append(intToString(current_index));

}


void TimerProgram::next(){
  current_index++;
  //message = intToString(current_index);

}

void TimerProgram::processPassedData(){
  
  if(hasDataBeenPassed()){
    std::vector<DesktopicoProgram*> LOL = *(std::vector<DesktopicoProgram*>*)getDataPtr();
    auto itter = LOL.begin();
    auto endOfVector = LOL.end();
    while(itter != endOfVector){
      listOfPrograms.emplace_back((*itter)->getDisplayableName());
      itter++;
    }
    listOfPrograms.emplace_back("OPTION_1");
  }

}

std::string TimerProgram::intToString(int value){
  return std::to_string(value);
}

