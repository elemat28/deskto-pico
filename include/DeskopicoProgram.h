#ifndef DESKTOPRGRM_H
#define DESKTOPRGRM_H
#include "UIButton.h"

#ifndef MAXPICOPROGRAMIDCHARS 
#define MAXPICOPROGRAMIDCHARS 16
#endif
class DeskopicoProgram
{
  
protected:
char _programID[MAXPICOPROGRAMIDCHARS+1];
  int _finalIDCharIndex;
  UIButton* _buttons;
  void* _dataObject;
  bool _hasDataBeenPassed;
  void* getDataPtr();
public:

  bool hasDataBeenPassed();
  DeskopicoProgram(char program_name[]);
  void pass_buttons(UIButton buttons[]);
  void pass_data(void* dataObject);
  virtual void run() = 0;
  int getNumOfCharsInID();
  

  
  
};


#endif