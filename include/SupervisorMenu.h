#ifndef UISUPRVSRMNU_H
#define UISUPRVSRMNU_H
#include "DeskopicoProgram.h"
class SupervisorMenu: public DesktopicoProgram
{
public:
  SupervisorMenu();
  SupervisorMenu(DesktopicoProgram* programs);
  SupervisorMenu(const DesktopicoProgram* programs): SupervisorMenu((DesktopicoProgram*)programs){};
  ~SupervisorMenu();
 ProgramReturn* run(UIButtonSet* availableButtons);
  void select();
  void previous();
  void next();
protected:
  static std::string ID;
  std::string intToString(int value);
private:
  volatile int clicks;
  std::string message;


  
  
};


#endif