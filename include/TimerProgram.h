#ifndef TMRPRGRM_H
#define TMRPRGRM_H
#include "DesktopicoProgram.h"
#include "Timer.h"
class TimerProgram : public DesktopicoProgram
{
public:
  TimerProgram();
  ~TimerProgram();
  ProgramReturn *run(int *refresh_after_ms);
  void select();
  void previous();
  void next();
  void init();

protected:
  static std::string static_ID;
  std::string intToString(int value);

private:
  enum SCREEN
  {
    QUICKSELECT,
    RUNNING,
    CUSTOM
  };
  Timer timerObject;
  SCREEN current_screen;
  ProgramReturn quickselect_return;
  ProgramReturn running_return;
  ProgramReturn custom_return;
  std::function<void()> supervisor_target;
  void processPassedData();
  std::vector<std::string> listOfPrograms;
  int current_index;
  std::string message;
  OPTION_BUTTONS_STRUCT quickselect_return_data_as_option_buttons;
  OPTION_BUTTONS_STRUCT running_return_data_as_option_buttons;
  void create_timer(int seconds);
  std::string formatTimerTimeLeftToString(Timer *timerObj);

  void QUICKSELECT_configure();
  void QUICKSELECT_button_return();
  void QUICKSELECT_button_select();
  void QUICKSELECT_button_next();

  void RUNNING_configure();
  void RUNNING_button_return();
  void RUNNING_button_select();
  void RUNNING_button_next();
};

#endif