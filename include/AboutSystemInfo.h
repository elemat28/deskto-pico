#ifndef UISYSINFO_H
#define UISYSINFO_H
#include "DesktopicoProgram.h"

struct BasicRequiredInfo {
  std::string OS_NAME;
  float version;
  std::string author_name;
  BasicRequiredInfo(std::string project_name, float currentVersion, std::string author){
    OS_NAME = project_name;
    version = currentVersion;
    author_name = author;
  };
};
class AboutSystemInfo: public DesktopicoProgram
{
public:

static std::string ID;
static std::string displayAs;
  AboutSystemInfo(BasicRequiredInfo baseInfo);
  std::map<std::string, std::string> systemDetails;
  void init();
  ProgramReturn* run(UIButtonSet* availableButtons);
  void select();
  void previous();
  void next();

  
};


#endif