#ifndef UISUPRVSRMNU_H
#define UISUPRVSRMNU_H
#include "DeskopicoProgram.h"
class SupervisorMenu: public DeskopicoProgram
{
public:
  SupervisorMenu(DeskopicoProgram* programs);
  SupervisorMenu(const DeskopicoProgram* programs): SupervisorMenu((DeskopicoProgram*)programs){};
  ~SupervisorMenu();
  void run();


  
  
};


#endif