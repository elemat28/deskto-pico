#ifndef SERIALUI_H
#define SERIALUI_H
#include "UIDisplayHandler.h"
#include "SerialUART.h"

class DEBUGSerialUIDisplay : public UIDisplayHandler
{

public:
  static SUPPORTED_FORMATS DISPLAY_FORMATS;
  virtual void output_auto(ProgramReturn *programOtput);
  DEBUGSerialUIDisplay(SerialUART *serial);
  DEBUGSerialUIDisplay();
  int init();
  void clear();
  void safe_output(char *data);
  void safe_output(const char *data);

  // void output_auto(ProgramReturn* programOtput);
protected:
  void output(ProgramReturn *);
  void center_output(int row, std::string *output);
  void center_output_with_arrows(int row, std::string *output, bool retArr = true, bool nextArr = true);
  void display_as_simple_list(ProgramReturn *programOtput);
  void display_as_key_value_list(ProgramReturn *programOtput);
  void display_as_option_buttons(ProgramReturn *programOtput);

private:
  SerialUART *screen;
};

#endif