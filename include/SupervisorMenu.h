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
 ProgramReturn* run(bool* availableButtons);
  void select();
  void previous();
  void next();
  void init();
  int* set_supervisor_funct(std::function<void()>* target);
  int  get_number_of_available_programs();
  bool set_current_index(int index);
protected:
  static std::string static_ID;
  std::string intToString(int value);
private:
  std::function<void()> supervisor_target;
  void processPassedDataToProgramList();
  std::vector<std::string> listOfPrograms;
  int current_index;
  std::string message;
  LIST_OPTIONS_SIMPLE_STRUCT return_data;


  
  
};


#endif