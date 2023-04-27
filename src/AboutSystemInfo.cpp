#include "AboutSystemInfo.h"
std::string AboutSystemInfo::ID("OS_INFO");
AboutSystemInfo::AboutSystemInfo():DesktopicoProgram(ID) {
}
 
ProgramReturn* AboutSystemInfo::run(UIButtonSet* availableButtons){
    return &returnValue;
};






