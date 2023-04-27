#ifndef UIBUTTSETS_H
#define UIBUTTSETS_H
#include "UIButton.h"
#include <map>


typedef std::pair<std::string, UIButton> DeclaredButton;
struct UIButtonSet
{
  static DeclaredButton RETURN;
  static DeclaredButton SELECT;
  static DeclaredButton NEXT;
};

struct fullSet: UIButtonSet {
  static DeclaredButton PREVIOUS;
};



#endif