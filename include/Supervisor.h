#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include <DesktoPicoENV.h>
#include "AboutSystemInfo.h"
#include "SupervisorMenu.h"
#include "UIDisplayHandler.h"
#include "RgbLED.h"
#include "PicoBuzzer.h"
#include <cstring>

#ifndef UISUPRVSRMAXPRGRMS
#define UISUPRVSRMAXPRGRMS 6
#endif

class Supervisor : public UINotification
{
public:
  UIButtonSet REQUIRED_BUTTONS;
  Supervisor();
  Supervisor(BasicRequiredInfo INFO);
  Supervisor(DesktopicoProgram *programs);
  ~Supervisor();
  int setBaseButtonGPIO(UIButton button, int GPIO);
  bool splashScreenDuringStartup();
  void splashScreenDuringStartup(bool newSettting);
  int splashScreen_min_ms();
  void splashScreen_min_ms(int minimum_ms);
  void add_program(DesktopicoProgram *program, size_t programSize);
  bool passDataToProgramID(std::string programID, void *data);
  void set_UIDisplay(UIDisplayHandler *display);
  void set_workQueue(queue_t *queue_ptr);
  void set_RGBLed(RgbLED &object);
  void set_Buzzer(PicoBuzzer &object);
  void assign_alarm_pool(alarm_pool_t *pass_ptr);
  void set_startup_program(char name[]);
  void startup_begin();
  void finalize();
  void startup_finish();
  void run();
  void run(bool forceRefresh);
  bool hasWork();
  bool peekhasWork();
  void run_program(char name[]);
  int debugFunc();
  int debugFunc(void *data);
  std::string getLogs();
  void return_to_menu();
  UIButton HOME;

protected:
  int _blip_info();
  int _blip_warn();
  int _blip_error();
  int _blip_success();
  int _short_error();
  int _short_success();
  int _state_default();
  int _state_alert();
  int _state_alarm();
  int _state_waiting();
  int _await_info(uint optional_timeout);
  int _await_warn(uint optional_timeout);
  int _await_error(uint optional_timeout);
  int _await_choice(uint optional_timeout);
  std::vector<SongNote> SHORT_ERROR;

private:
  UIButton HOME_BUTTON;
  std::string logMessage;
  std::function<void()> target;
  int *supervisorMenuTargetIndex;
  void prep_target();
  void change_run_target();
  float _ver;
  absolute_time_t _endOfSplashScreen;
  bool _splashScreenDuringStartup;
  unsigned int _splashScreenMinLenMS;
  volatile bool _pendingButton;
  volatile int _pressedIndex;
  bool _pendingScreenRefresh;
  volatile bool _hasTargetOutputChanged;
  alarm_pool_t *alarmPool_ptr;
  uint POOL_ID;
  long int NUM_OF_TIMERS;
  repeating_timer repeatingRefreshTimer;
  alarm_id_t _refresh_alarm_ID;
  void _trigger_return();
  void _trigger_select();
  void _trigger_next();
  void _return_to_main_menu();
  bool _auto_refresh_live;
  volatile int _program_refresh_ms;
  absolute_time_t _auto_refresh;
  SupervisorMenu OS_MENU;
  AboutSystemInfo SYS_INFO;
  std::vector<DesktopicoProgram *> myPrograms;
  DesktopicoProgram *_currentRunTarget;
  ProgramReturn *volatile returnedOutput;
  bool finalized;
  UIButton _assignableButtons;
  // DeskopicoProgram _programs[1];
  UIButton _virtualButtons;
  DesktopicoProgram *programDeclarations[UISUPRVSRMAXPRGRMS];

  DesktopicoProgram *temp_startupTarget;
  DesktopicoProgram *startupTarget;

  DesktopicoProgram *temp_arrayOfPrograms;
  DesktopicoProgram *arrayOfPrograms;

  UIDisplayHandler *hardwareDisplay;
  UIDisplayHandler *temp_hardwareDisplay;

  RgbLED notificationLED;
  PicoBuzzer buzzer;

  queue_t *workQueue;
  queue_t *temp_workQueue;

  int temp_numOfPrograms;
  int numOfPrograms;
  RefreshTimerData data;

  void create_alarm_pool();
  void destroy_and_recreate_alarm_pool();
};

#endif