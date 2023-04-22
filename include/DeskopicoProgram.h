#ifndef DESKTOPRGRM_H
#define DESKTOPRGRM_H
#include "UIButton.h"
class DeskopicoProgram
{
protected:
  void* dataObject;
  char programName[];
public:
  DeskopicoProgram(UIButton buttons[]);
  ~DeskopicoProgram();
  void passData(void* dataObject);
  void run();


  
  
};


#endif