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
  OUTPUT_FORMAT formatOfData;
  UIButtonSet buttonSet;
  void* data;
  
};
class DesktopicoProgram
{
  
protected:
  char _programID[MAXPICOPROGRAMIDCHARS+1];
  int _finalIDCharIndex;
  UIButton* _buttons;
  void* _dataObject;
  bool _hasDataBeenPassed;
  void* getDataPtr();
  ProgramReturn returnValue;
public:
  bool hasDataBeenPassed();
  DesktopicoProgram(char program_name[]);
  void pass_buttons(UIButton buttons[]);
  void pass_data(void* dataObject);
  virtual ProgramReturn* run(UIButtonSet* availableButtons) = 0;
  int getNumOfCharsInID();
};


#endif