#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include "UIButtonSet.h"
#include "AboutSystemInfo.h"
#include "SupervisorMenu.h"
#include "UIDisplayHandler.h" 
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
  void add_function(DesktopicoProgram* program);
  void set_UIDisplay(UIDisplayHandler* display);
  void set_startup_program(char name[]);
  void finalize();
  void startup();
  void run();
  bool hasWork();
  bool peekhasWork();
  void run_program(char name[]);
  int debugFunc();
  int debugFunc(void* data);
   static void GPIOInterruptHandler_SINGULAR();
  static void GPIOInterruptHandler_RETURN();
  static void GPIOInterruptHandler_SELECT();
  static void GPIOInterruptHandler_NEXT();
  std::string getLogs();
private:
  std::string logMessage;
  std::function<void()> target;
  int* supervisorMenuTargetIndex;
  void prep_target();
  void change_run_target();
  float _ver;
  bool _splashScrenDuringStartup;
  volatile bool _pendingButton;
  volatile int _pressedIndex;
  bool _pendingScreenRefresh;
  void _trigger_return();
  void _trigger_select();
  void _trigger_next();
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