#ifndef DESKTOPRGRM_H
#define DESKTOPRGRM_H
#include <UIButtonSet.h>

#ifndef MAXPICOPROGRAMIDCHARS 
#define MAXPICOPROGRAMIDCHARS 16
#endif
enum OUTPUT_FORMAT {
  BASE,
  LIST 
};

struct ProgramReturn {
  OUTPUT_FORMAT volatile formatOfData;
  UIButtonSet* volatile buttonSet;
  void* volatile data;
  
};
class DesktopicoProgram
{
  
protected:
  std::function<void(void)> return_button_funct;
  std::function<void(void)> select_button_funct;
  std::function<void(void)> next_button_funct;
  char _programID[MAXPICOPROGRAMIDCHARS+1];
  int _finalIDCharIndex;
  UIButton* _buttons;
  void* _dataObject;
  bool _hasDataBeenPassed;
  void* getDataPtr();
  ProgramReturn  returnValue;
public:
  UIButtonSet  ProgramDefinedButtons;
  bool hasDataBeenPassed();
  DesktopicoProgram(char program_name[]);
  void pass_buttons(UIButton buttons[]);
  void pass_data(void* dataObject);
  virtual ProgramReturn* run(UIButtonSet* availableButtons) = 0;
  int getNumOfCharsInID();
};


#endif