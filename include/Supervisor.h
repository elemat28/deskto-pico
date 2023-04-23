#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include "DeskopicoProgram.h"
#include "SupervisorMenu.h"
#ifndef UISUPRVSRMAXPRGRMS
#define UISUPRVSRMAXPRGRMS 6
#endif


  enum BASE_BUTTONS {
  BUTTON_RETURN  = 0,
  BUTTON_SELECT  = 1,
  BUTTON_NEXT    = 2
}; 
class Supervisor
{
private:
  float _ver;
  UIButton RETURN_BUTTON;
  UIButton SELECT_BUTTON;
  SupervisorMenu OS_MENU;
  DeskopicoProgram* _currentRunTarget;
  bool finalized;
  UIButton _assignableButtons;
  //DeskopicoProgram _programs[1];
  UIButton _virtualButtons;
  DeskopicoProgram* programDeclarations[UISUPRVSRMAXPRGRMS];

  DeskopicoProgram* temp_startupTarget;
  DeskopicoProgram* startupTarget;

  DeskopicoProgram* temp_arrayOfPrograms;
  DeskopicoProgram* arrayOfPrograms;

  int temp_numOfPrograms;
  int numOfPrograms;
public:
  static BASE_BUTTONS REQUIRED_BUTTONS;
  Supervisor();
  Supervisor(DeskopicoProgram* programs);
  ~Supervisor();
  int setBaseButtonGPIO(BASE_BUTTONS baseButton, int GPIO);
  void add_function(DeskopicoProgram* program);
  void set_startup_program(char name[]);
  void run_program(char name[]);
  void finalize();
  void run();
  int debugFunc();
  int debugFunc(void* data);
};


#endif