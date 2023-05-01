#ifndef DESKTOPRGRM_H
#define DESKTOPRGRM_H
#include <UIButtonSet.h>
#include <vector>
#ifndef MAXPICOPROGRAMIDCHARS 
#define MAXPICOPROGRAMIDCHARS 16
#endif
enum OUTPUT_FORMAT {
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
struct LIST_OPTIONS_SIMPLE_STRUCT {
  int* INDEX;
  std::vector<std::string> OPTIONS_VECTOR;
  LIST_OPTIONS_SIMPLE_STRUCT(int* index, std::vector<std::string> options){
    INDEX = index;
    OPTIONS_VECTOR = options;
  }
  LIST_OPTIONS_SIMPLE_STRUCT(){
    INDEX = nullptr;

  }
};

typedef std::vector<OUTPUT_FORMAT> SUPPORTED_FORMATS;
struct ProgramReturn {
  std::string PROGRAM_ID;
  OUTPUT_FORMAT volatile formatOfData;
  SUPPORTED_FORMATS* FORMAT_PREFERENCE;
  UIButtonSet* volatile buttonSet;
  void* data;
  ProgramReturn(){
    formatOfData = U_DEF;
    PROGRAM_ID = nullptr;
    FORMAT_PREFERENCE =  nullptr;
    buttonSet = nullptr;
    
  }
  
};


class DesktopicoProgram
{
  
protected:

  
  std::function<void()> return_button_funct;
  std::function<void()> select_button_funct;
  std::function<void()> next_button_funct;
  char _programID[MAXPICOPROGRAMIDCHARS+1];
  int _finalIDCharIndex;
  UIButton* _buttons;
  void* _dataObject;
  bool _hasDataBeenPassed;
  void* getDataPtr();
  ProgramReturn returnValue;
  //void setID(std::string programID);

public:
  virtual void init() = 0;
  virtual ProgramReturn* run(UIButtonSet* availableButtons) = 0;
  SUPPORTED_FORMATS FORMAT_PRIORITY;
  UIButtonSet  ProgramDefinedButtons;
  bool hasDataBeenPassed();
  DesktopicoProgram(std::string program_ID);
  //DesktopicoProgram(std::string program_ID);
  void pass_buttons(UIButton buttons[]);
  void pass_data(void* dataObject);
  int getNumOfCharsInID();
  std::string static getID();
  static std::string ID;
  static std::string display_as;
  std::string static getDisplayableName();
private:
 
};


#endif