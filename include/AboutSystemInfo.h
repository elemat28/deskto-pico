#ifndef UISYSINFO_H
#define UISYSINFO_H
#include "DeskopicoProgram.h"


class AboutSystemInfo: public DesktopicoProgram
{
public:
  AboutSystemInfo();
  static char name[];
  ProgramReturn* run(UIButtonSet* availableButtons);
  std::map<std::string, std::string> systemDetails;
private:
};


#endif