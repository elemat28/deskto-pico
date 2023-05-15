#ifndef I2COLEDUIDSPL_H
#define I2COLEDUIDSPL_H
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "UIDisplayHandler.h"
#ifndef DEFAULTOLEDI2CADDR
#define DEFAULTOLEDI2CADDR 0x3c
#endif

class OLEDUIDisplay : public UIDisplayHandler
{

public:
  static SUPPORTED_FORMATS DISPLAY_FORMATS;
  typedef struct OLEDUIDisplay_t
  {
    OLEDUIDisplay_t();
    OLEDUIDisplay_t(int I2Caddress, int width, int height);
    int I2Caddress;
    int width_px;
    int height_px;
  } OLEDUIDisplay_t;
  virtual void output_auto(ProgramReturn *programOtput);
  static OLEDUIDisplay_t DEFAULTDISPLAYCONFIG;
  OLEDUIDisplay();
  OLEDUIDisplay(OLEDUIDisplay_t displayDefinition);
  int init();
  void clear();
  void safe_output(char *data);
  void safe_output(const char *data);

  // void output_auto(ProgramReturn* programOtput);
protected:
  void output(ProgramReturn *);
  void center_output(int row, std::string *output);
  void center_output_with_arrows(int row, std::string *output, bool retArr = true, bool nextArr = true);
  OLEDUIDisplay_t currentLCDConfig;
  std::vector<std::string> display_rows;
  void display_as_simple_list(ProgramReturn *programOtput);
  void display_as_key_value_list(ProgramReturn *programOtput);
  void display_as_option_buttons(ProgramReturn *programOtput);

private:
  Adafruit_SSD1306 screen;
  int _i2cAdrrs;
  bool _backlight;
};

#endif