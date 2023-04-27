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
  void set_value();
  void clear_value();
  void set_str();
protected:
  static std::string ID;
private:
  std::string message;


  
  
};


#endif