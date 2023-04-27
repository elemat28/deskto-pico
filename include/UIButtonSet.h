#ifndef UIBUTTSETS_H
#define UIBUTTSETS_H
#include "UIButton.h"
#include <map>


struct UIButtonSet
{
  UIButton RETURN;
  UIButton SELECT;
  UIButton NEXT;
  UIButtonSet(){
    RETURN = UIButton("OS_RETURN");
    SELECT = UIButton("OS_SELECT");
    NEXT   = UIButton("OS_NEXT");
  }
};

struct fullSet: UIButtonSet {
  UIButton PREVIOUS;
};

static UIButtonSet BASE_SET;


#endif