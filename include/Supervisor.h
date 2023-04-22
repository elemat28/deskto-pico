#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include "UIButton.h"
#include "DeskopicoProgram.h"
class Supervisor
{
private:
  float _ver;
  UIButton RETURN_BUTTON;
  UIButton SELECT_BUTTON;
  UIButton _assignableButtons;
  UIButton _virtualButtons;
  DeskopicoProgram* startupTarget;
public:
  Supervisor();
  ~Supervisor();
  void add_function(DeskopicoProgram program);
  void set_startup_program(char name[]);
  void run_program(char name[]);


};


#endif