#ifndef UISYSINFO_H
#define UISYSINFO_H
#include "DeskopicoProgram.h"
#include <map>

class AboutSystemInfo: public DeskopicoProgram
{
public:
  AboutSystemInfo();
  static char name[];
  void run();
  std::map<std::string, std::string> systemDetails;
private:
};


#endif