#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include "AboutSystemInfo.h"
#include "SupervisorMenu.h"
#include "UIDisplayHandler.h" 
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
public:
  static BASE_BUTTONS REQUIRED_BUTTONS;
  Supervisor();
  Supervisor(DeskopicoProgram* programs);
  ~Supervisor();
  int setBaseButtonGPIO(BASE_BUTTONS baseButton, int GPIO);
  void add_function(DeskopicoProgram* program);
  void set_UIDisplay(UIDisplayHandler* display);
  void set_startup_program(char name[]);
  void finalize();
  void startup();
  void run();
  void run_program(char name[]);
  int debugFunc();
  int debugFunc(void* data);

private:
  float _ver;
  bool _splashScrenDuringStartup;
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

  UIDisplayHandler* hardwareDisplay;
  UIDisplayHandler* temp_hardwareDisplay;

  int temp_numOfPrograms;
  int numOfPrograms;
};


#endif