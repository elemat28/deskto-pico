#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include "DeskopicoProgram.h"
#include "SupervisorMenu.h"
#ifndef UISUPRVSRMAXPRGRMS
#define UISUPRVSRMAXPRGRMS 6
#endif
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
  Supervisor();
  Supervisor(DeskopicoProgram* programs);
  ~Supervisor();
  void add_function(DeskopicoProgram* program);
  void set_startup_program(char name[]);
  void run_program(char name[]);
  void finalize();
  void run();
  int debugFunc();
  int debugFunc(void* data);
};


#endif