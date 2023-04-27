#ifndef UISYSINFO_H
#define UISYSINFO_H
#include "DeskopicoProgram.h"


class AboutSystemInfo: public DesktopicoProgram
{
public:
static std::string ID;
  AboutSystemInfo();
  ProgramReturn* run(UIButtonSet* availableButtons);
  std::map<std::string, std::string> systemDetails;
protected:
  
};


#endif