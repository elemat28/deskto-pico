#ifndef KEYINPUTPRGRM_H
#define KEYINPUTPRGRM_H
#include "DesktopicoProgram.h"
class AutoLoginProgram : public DesktopicoProgram
{
public:
  struct AccountDetails
  {
    std::string displayName;
    std::string login;
    std::string password;
    AccountDetails()
    {
      displayName = std::string("NULL");
      login = std::string("USERNAME");
      password = std::string("PASSWORD");
    };
    AccountDetails(std::string label, std::string username, std::string psswd)
    {
      displayName = label;
      login = username;
      password = psswd;
    };
  };
  AutoLoginProgram();
  ~AutoLoginProgram();
  ProgramReturn *run(int *refresh_after_ms);
  void select();
  void previous();
  void next();
  void init();

protected:
  static std::string static_ID;
  std::string intToString(int value);

private:
  enum SCREEN
  {
    ACCOUNTSELECT,
    OUTPUTSELECTION
  };
  SCREEN current_screen;
  ProgramReturn ACCOUNTSELECT_return;
  ProgramReturn OUTPUTSELECTION_return;
  void processPassedData();
  int current_index;
  std::string message;
  LIST_OPTIONS_SIMPLE_STRUCT accountselect_return_data;
  OPTION_BUTTONS_STRUCT outputselection_return_data;
  void ACCOUNTSELECT_configure();
  void QUICKSELECT_button_return();
  void QUICKSELECT_button_select();
  void QUICKSELECT_button_next();

  void OUTPUTSELECTION_configure();
  void RUNNING_button_return();
  void RUNNING_button_select();
  void RUNNING_button_next();
};

#endif