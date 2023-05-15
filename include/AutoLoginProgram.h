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

private:
  enum OutputMode
  {
    USERNAME,
    PASSWORD,
    FULL
  };
  OutputMode outputSetting;
  std::vector<AutoLoginProgram::AccountDetails> accouts;
  enum SCREEN
  {
    ACCOUNTSELECT,
    OUTPUTSELECTION
  };
  SUPPORTED_FORMATS ACCOUNTSELECT_formats_priority;
  SUPPORTED_FORMATS OUTPUTSELECTION_formats_priority;

  SCREEN current_screen;
  ProgramReturn ACCOUNTSELECT_return;
  ProgramReturn OUTPUTSELECTION_return;
  void processPassedData();
  int current_index;
  std::string message;
  LIST_OPTIONS_SIMPLE_STRUCT accountselect_return_data;
  OPTION_BUTTONS_STRUCT outputselection_return_data;
  void ACCOUNTSELECT_configure();
  void OUTPUTSELECTION_configure();
  void triggerOutput();
};

#endif