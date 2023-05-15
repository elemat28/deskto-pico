#ifndef UISUPRVSR_H
#define UISUPRVSR_H
#include <Arduino.h>
#include <pico.h>
#include <pico/time.h>
#include "pico/util/queue.h"
#include "UIButtonSet.h"
#include "AboutSystemInfo.h"
#include "SupervisorMenu.h"
#include "UIDisplayHandler.h"
#include <cstring>

#ifndef UISUPRVSRMAXPRGRMS
#define UISUPRVSRMAXPRGRMS 6
#endif

enum TypeOfWork
{
  NONE,
  INTERRUPT,
  BUTTON,
  REFRESH,
  TIMEOUT
};
struct PendingWork
{
  TypeOfWork TYPE;
  int PENDING_OBJECT_ID;
  void *data;
  PendingWork()
  {
    TYPE = NONE;
    PENDING_OBJECT_ID = -1;
    data = nullptr;
  };
  PendingWork(TypeOfWork type, int ID) : PendingWork()
  {
    TYPE = type;
    PENDING_OBJECT_ID = ID;
  };
  PendingWork(TypeOfWork type, uint ID) : PendingWork()
  {
    TYPE = type;
    PENDING_OBJECT_ID = ID;
  };
  PendingWork(TypeOfWork type, int ID, void *data_ptr)
  {
    TYPE = type;
    PENDING_OBJECT_ID = ID;
    data = data_ptr;
  };
};

struct RefreshTimerData
{
  alarm_pool_t *alarmPool;
  alarm_id_t *alarmID;
  bool *screenRefreshPending;
  RefreshTimerData(alarm_pool_t *pool_ptr, alarm_id_t *alarmID_ptr, bool *screenRefresh_ptr)
  {
    alarmPool = pool_ptr;
    alarmID = alarmID_ptr;
    screenRefreshPending = screenRefresh_ptr;
  }
  RefreshTimerData()
  {
    alarmPool = nullptr;
    alarmID = nullptr;
    screenRefreshPending = nullptr;
  }
};

class Supervisor
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

  queue_t *workQueue;
  queue_t *temp_workQueue;

  int temp_numOfPrograms;
  int numOfPrograms;
  RefreshTimerData data;

  void create_alarm_pool();
  void destroy_and_recreate_alarm_pool();
};

#endif