#include "AboutSystemInfo.h"
char AboutSystemInfo::name[] = "SYS_INFO";
AboutSystemInfo::AboutSystemInfo():DeskopicoProgram(name) {

}



void AboutSystemInfo::run(){
  
  if(hasDataBeenPassed()){
    *((int*)getDataPtr()) += 1;
  }
  
}



