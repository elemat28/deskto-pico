#ifndef I2CLCUIDSPL_H
#define I2CLCUIDSPL_H
#include <LiquidCrystal_I2C.h>
#include "UIDisplayHandler.h"
#ifndef DEFAULTLCI2CADDR
#define DEFAULTLCI2CADDR 0x27
#endif



class LCUIDisplay: public UIDisplayHandler {



  public:
 static SUPPORTED_FORMATS DISPLAY_FORMATS;
  typedef struct LCI2CDisplay_t {
    LCI2CDisplay_t();
    LCI2CDisplay_t(int I2Caddress, int columns, int lines);
    int I2Caddress;
    int columns;
    int lines;
  } LCI2CDisplay_t;
  virtual void output_auto(ProgramReturn* programOtput);
  static LCI2CDisplay_t DEFAULTDISPLAYCONFIG;
    LCUIDisplay();
    LCUIDisplay(LCI2CDisplay_t displayDefinition);
    int init();
    void clear();
    void safe_output(char* data);
    void safe_output(const char* data);
    //void output_auto(ProgramReturn* programOtput);
protected:
    void output(ProgramReturn*);
    void center_output(int row, std::string* output);
    void center_output_with_arrows(int row, std::string* output, bool retArr = true, bool nextArr = true);
    LCI2CDisplay_t currentLCDConfig;
    std::vector<std::string> display_rows;
    void display_as_simple_list(ProgramReturn* programOtput);
    void display_as_key_value_list(ProgramReturn* programOtput);
     void display_as_option_buttons(ProgramReturn* programOtput);
private:
    typedef LiquidCrystal_I2C obj;
    LiquidCrystal_I2C screen;
    int _i2cAdrrs; 
    bool _backlight;

};


#endif