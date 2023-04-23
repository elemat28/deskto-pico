#include "SupervisorMenu.h"
char name[] = "OS_MENU";
SupervisorMenu::SupervisorMenu(DeskopicoProgram* programs): DeskopicoProgram(name) {

}


SupervisorMenu::~SupervisorMenu(){

}

void SupervisorMenu::run(){
  
  if(hasDataBeenPassed()){
    *((int*)getDataPtr()) += 1;
  }
  
}



