#ifndef DESKTOPICOENV_H
#define DESKTOPICOENV_H
#include <Arduino.h>
#include <pico.h>
#include <pico/time.h>
#include <cstring>
#include <vector>
#include <map>
#include "pico/util/queue.h"
#include "UIButtonSet.h"

#ifndef MAXPICOPROGRAMIDCHARS
#define MAXPICOPROGRAMIDCHARS 16
#endif

#ifndef UISUPRVSRMAXPRGRMS
#define UISUPRVSRMAXPRGRMS 6
#endif

enum OUTPUT_FORMAT
{
    U_DEF,
    BASE,
    OPTION_BOOLEAN,
    LIST_OPTIONS_SIMPLE,
    LIST_OPTIONS_INDEXED,
    KEY_VALUE_LIST_SIMPLE,
    KEY_VALUE_LIST_INDEXED,
    KEY_OPTION_SCROLL,
    KEY_OPTION_GRANULAR,
    OPTION_BUTTONS,
    HEADING_LIST
};

enum TypeOfWork
{
    NONE,
    INTERRUPT,
    BUTTON,
    REFRESH,
    TIMEOUT
};

typedef std::vector<OUTPUT_FORMAT> SUPPORTED_FORMATS;

struct LIST_OPTIONS_SIMPLE_STRUCT
{
    int *INDEX;
    std::vector<std::string> OPTIONS_VECTOR;
    LIST_OPTIONS_SIMPLE_STRUCT(int *index, std::vector<std::string> options)
    {
        INDEX = index;
        OPTIONS_VECTOR = options;
    }
    LIST_OPTIONS_SIMPLE_STRUCT()
    {
        INDEX = nullptr;
    }
};

struct KEY_VALUE_LIST_SIMPLE_STRUCT
{
    int *INDEX;
    std::map<std::string, std::string> OPTIONS_MAP;
    std::vector<std::pair<std::string, std::string>> OPTIONS_VECTOR;
    KEY_VALUE_LIST_SIMPLE_STRUCT(int *index, std::map<std::string, std::string> key_value)
    {
        INDEX = index;
        OPTIONS_MAP = key_value;
    }
    KEY_VALUE_LIST_SIMPLE_STRUCT()
    {
        INDEX = nullptr;
        OPTIONS_VECTOR.clear();
    }
    std::vector<std::pair<std::string, std::string>> as_vector()
    {
        auto_vector();
        return OPTIONS_VECTOR;
    }
    void auto_vector()
    {
    }
};

struct OPTION_BUTTONS_STRUCT
{
    std::string message;
    UIButtonSet buttons;
    OPTION_BUTTONS_STRUCT()
    {
        message = std::string();
        buttons = UIButtonSet();
    };
    OPTION_BUTTONS_STRUCT(std::string label_message) : OPTION_BUTTONS_STRUCT()
    {
        message = label_message;
    };
};

struct ProgramReturn
{
    std::string PROGRAM_ID;
    OUTPUT_FORMAT volatile formatOfData;
    SUPPORTED_FORMATS *FORMAT_PREFERENCE;
    UIButtonSet *volatile buttonSet;
    void *data;
    int refresh_freq_ms;
    ProgramReturn()
    {
        formatOfData = U_DEF;
        PROGRAM_ID = nullptr;
        FORMAT_PREFERENCE = nullptr;
        buttonSet = nullptr;
        refresh_freq_ms = -1;
    }
};

struct BasicRequiredInfo
{
    std::string OS_NAME;
    float version;
    std::string author_name;
    BasicRequiredInfo(std::string project_name, float currentVersion, std::string author)
    {
        OS_NAME = project_name;
        version = currentVersion;
        author_name = author;
    };
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

class UINotification
{
public:
    static enum OS_NOTIFICATION_STATUS {
        DEFAULT,
        WAITING,
        ALERT,
        ALARM,
        TEMP_SUCCESS,
        TEMP_ERROR,
        AWAIT_INFO,
        AWAIT_CHOICE,
        AWAIT_WARN,
        AWAIT_ERROR

    } OS_NOTIFICATION_STATUS_ENUM;
    const OS_NOTIFICATION_STATUS get_current_state() { return current_notifcation_state; };
    const bool switch_to_status(OS_NOTIFICATION_STATUS newState)
    {
        if (current_notifcation_state != newState)
        {
            current_notifcation_state = newState;
            return true;
        }
        else
        {
            return false;
        };
    };

    int blip_info() { return _blip_info(); };
    int blip_warn() { return _blip_warn(); };
    int blip_error() { return _blip_error(); };
    int blip_success() { return _blip_success(); };
    int short_error()
    {
        if (switch_to_status(TEMP_ERROR))
            return _short_error();
        return 0;
    };
    int short_success()
    {
        if (switch_to_status(TEMP_SUCCESS))
            return _short_success();
        return 0;
    };
    int state_default()
    {
        if (switch_to_status(DEFAULT))
            return _state_default();
        return 0;
    };
    int state_alert()
    {
        if (switch_to_status(ALERT))
            return _state_alert();
        return 0;
    };
    int state_alarm()
    {
        if (switch_to_status(ALARM))
            return _state_alarm();
        return 0;
    };
    int state_waiting()
    {
        if (switch_to_status(WAITING))
            return _state_waiting();
        return 0;
    };
    int await_info(uint optional_timeout)
    {
        if (switch_to_status(AWAIT_INFO))
            return _await_info(optional_timeout);
        return 0;
    };
    int await_choice(uint optional_timeout)
    {
        if (switch_to_status(AWAIT_CHOICE))
            return _await_choice(optional_timeout);
        return 0;
    };
    int await_warn(uint optional_timeout)
    {
        if (switch_to_status(AWAIT_WARN))
            return _await_warn(optional_timeout);
        return 0;
    };
    int await_error(uint optional_timeout)
    {
        if (switch_to_status(AWAIT_ERROR))
            return _await_error(optional_timeout);
        return 0;
    };

protected:
    OS_NOTIFICATION_STATUS current_notifcation_state;
    virtual int _blip_info() = 0;
    virtual int _blip_warn() = 0;
    virtual int _blip_error() = 0;
    virtual int _blip_success() = 0;
    virtual int _short_error() = 0;
    virtual int _short_success() = 0;
    virtual int _state_default() = 0;
    virtual int _state_alert() = 0;
    virtual int _state_alarm() = 0;
    virtual int _state_waiting() = 0;
    virtual int _await_info(uint optional_timeout) = 0;
    virtual int _await_warn(uint optional_timeout) = 0;
    virtual int _await_error(uint optional_timeout) = 0;
    virtual int _await_choice(uint optional_timeout) = 0;
};

extern UINotification *supervisor_ptr;
#endif