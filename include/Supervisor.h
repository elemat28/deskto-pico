#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include "UIButtonSet.h"
#include "AboutSystemInfo.h"
#include "SupervisorMenu.h"
#include "UIDisplayHandler.h" 
#include <cstring>
#include <pico/time.h>
#ifndef UISUPRVSRMAXPRGRMS
#define UISUPRVSRMAXPRGRMS 6
#endif



class Supervisor
{
public:
  UIButtonSet REQUIRED_BUTTONS;
  Supervisor();
  Supervisor(DesktopicoProgram* programs);
  ~Supervisor();
  int setBaseButtonGPIO(UIButton button, int GPIO);
  bool splashScreenDuringStartup();
  void splashScreenDuringStartup(bool newSettting);
  int  splashScreen_min_ms();
  void splashScreen_min_ms(int minimum_ms);
  void add_program(DesktopicoProgram* program, size_t programSize);
  void set_UIDisplay(UIDisplayHandler* display);
  void set_startup_program(char name[]);
  void startup_begin();
  void finalize();
  void startup_finish();
  void run();
  bool hasWork();
  bool peekhasWork();
  void run_program(char name[]);
  int debugFunc();
  int debugFunc(void* data);
  std::string getLogs();
  void return_to_menu();
  UIButton HOME;
private:
  UIButton HOME_BUTTON;
  std::string logMessage;
  std::function<void()> target;
  int* supervisorMenuTargetIndex;
  void prep_target();
  void change_run_target();
  float _ver;
  absolute_time_t _endOfSplashScreen;
  bool _splashScreenDuringStartup;
  unsigned int _splashScreenMinLenMS;
  volatile bool _pendingButton;
  volatile int _pressedIndex;
  bool _pendingScreenRefresh;
  void _trigger_return();
  void _trigger_select();
  void _trigger_next();
  void _return_to_main_menu();
  SupervisorMenu OS_MENU;
  AboutSystemInfo SYS_INFO;
  std::vector<DesktopicoProgram*> myPrograms;
  DesktopicoProgram* _currentRunTarget;
  ProgramReturn* volatile returnedOutput;
  bool finalized;
  UIButton _assignableButtons;
  //DeskopicoProgram _programs[1];
  UIButton _virtualButtons;
  DesktopicoProgram* programDeclarations[UISUPRVSRMAXPRGRMS];

  DesktopicoProgram* temp_startupTarget;
  DesktopicoProgram* startupTarget;

  DesktopicoProgram* temp_arrayOfPrograms;
  DesktopicoProgram* arrayOfPrograms;

  UIDisplayHandler* hardwareDisplay;
  UIDisplayHandler* temp_hardwareDisplay;

  int temp_numOfPrograms;
  int numOfPrograms;
};


#endif