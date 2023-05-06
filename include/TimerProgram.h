#ifndef TMRPRGRM_H
#define TMRPRGRM_H
#include "DesktopicoProgram.h"
#include "Timer.h"
class TimerProgram: public DesktopicoProgram
{
public:
  TimerProgram();
  ~TimerProgram();
  ProgramReturn* run(UIButtonSet* availableButtons);
  void select();
  void previous();
  void next();
  void init();
  
protected:
  static std::string static_ID;
  std::string intToString(int value);
private:
  enum SCREEN {
    QUICKSELECT,
    RUNNING,
    CUSTOM
  };
  SCREEN current_screen;
  std::function<void()> supervisor_target;
  void processPassedData();
  std::vector<std::string> listOfPrograms;
  int current_index;
  std::string message;
  OPTION_BUTTONS_STRUCT quickselect_return_data;
  

  
  
};


#endif