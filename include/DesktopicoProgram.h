#ifndef DESKTOPRGRM_H
#define DESKTOPRGRM_H
#include <DesktoPicoENV.h>
#include <UIButtonSet.h>
#include <vector>
#ifndef MAXPICOPROGRAMIDCHARS
#define MAXPICOPROGRAMIDCHARS 16
#endif

class DesktopicoProgram
{

protected:
  std::function<void()> return_button_funct;
  std::function<void()> select_button_funct;
  std::function<void()> next_button_funct;
  char _programID[MAXPICOPROGRAMIDCHARS + 1];
  int _finalIDCharIndex;
  UIButton *_buttons;
  void *_dataObject;
  bool _hasDataBeenPassed;
  void *getDataPtr();
  ProgramReturn returnValue;
  // void setID(std::string programID);

public:
  virtual void init() = 0;
  virtual ProgramReturn *run(int *refresh_ms) = 0;
  SUPPORTED_FORMATS FORMAT_PRIORITY;
  UIButtonSet ProgramDefinedButtons;
  bool hasDataBeenPassed();
  DesktopicoProgram(std::string program_ID, std::string displayName);
  // DesktopicoProgram(std::string program_ID);
  void pass_buttons(UIButton buttons[]);
  void pass_data(void *dataObject);
  int getNumOfCharsInID();
  std::string getID();
  const std::string static_ID;
  const std::string displayAs;
  virtual std::string getDisplayableName();

private:
};

#endif