#include "TimerProgram.h"
const std::string ID = std::string("WORK_TIMER");
const std::string displayable = std::string("Timers");
TimerProgram::TimerProgram(): DesktopicoProgram(ID, displayable) {

  current_index = -1;
  
  FORMAT_PRIORITY = {LIST_OPTIONS_INDEXED, LIST_OPTIONS_SIMPLE, HEADING_LIST};
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
  
  return_data = LIST_OPTIONS_SIMPLE_STRUCT((int*)&current_index, listOfPrograms);
  returnValue.data = &return_data;

  
 
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
  processPassedDataToProgramList();
  
}




ProgramReturn* TimerProgram::run(UIButtonSet* availableButtons){
  //previous();
  
  return_data = LIST_OPTIONS_SIMPLE_STRUCT((int*)&current_index, listOfPrograms);
  returnValue.data = &return_data;
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

void TimerProgram::processPassedDataToProgramList(){
  
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
  else {
  
  listOfPrograms.emplace_back("OPTION2");
  listOfPrograms.emplace_back("OPTION_3X");
  listOfPrograms.emplace_back("OPTION_4XX");
  listOfPrograms.emplace_back("OPTION_5XXX");
  listOfPrograms.emplace_back("OPTION_6XXXX");
  listOfPrograms.emplace_back("OPTION_7XXXXX");
  }
}

std::string TimerProgram::intToString(int value){
  return std::to_string(value);
}

