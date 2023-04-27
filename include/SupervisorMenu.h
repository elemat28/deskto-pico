#ifndef UISUPRVSRMNU_H
#define UISUPRVSRMNU_H
#include "DeskopicoProgram.h"
class SupervisorMenu: public DesktopicoProgram
{
public:
  SupervisorMenu(DesktopicoProgram* programs);
  SupervisorMenu(const DesktopicoProgram* programs): SupervisorMenu((DesktopicoProgram*)programs){};
  ~SupervisorMenu();
 ProgramReturn* run(UIButtonSet* availableButtons);
  void set_value();
  void clear_value();
private:
  std::string message;

  
  
};


#endif