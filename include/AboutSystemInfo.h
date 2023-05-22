#ifndef UISYSINFO_H
#define UISYSINFO_H
#include "DesktopicoProgram.h"

class AboutSystemInfo : public DesktopicoProgram
{

public:
  AboutSystemInfo(BasicRequiredInfo baseInfo);
  std::map<std::string, std::string> systemDetails;
  void init();
  ProgramReturn *run(int *availableButtons);
  void select();
  void previous();
  void next();

private:
  KEY_VALUE_LIST_SIMPLE_STRUCT return_data;
  int index;
};

#endif