#ifndef UISUPRVSRMNU_H
#define UISUPRVSRMNU_H
#include "DesktopicoProgram.h"
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
  void init();

protected:
  static std::string static_ID;
  std::string intToString(int value);
private:
  void processPassedDataToProgramList();
  std::vector<std::string> listOfPrograms;
  volatile int current_index;
  std::string message;
  LIST_OPTIONS_SIMPLE_STRUCT return_data;


  
  
};


#endif