#ifndef UIDSPLHDNLR_H
#define UIDSPLHDNLR_H

enum class DISPLAY_FORMAT_T {
    BASIC_PRINT,
    UPDATEABLE_VALUE,
    SCROLLABLE_LIST,
    SCROLLABLE_VALUES,
    OPTION_BOOLEAN,
    OPTION_BUTTONS,
};


class UIDisplayHandler {
  public:
    const static DISPLAY_FORMAT_T DISPLAY_FORMAT;
    virtual void safe_output(char* data) = 0;
  protected:
    int declare_supported_formats(DISPLAY_FORMAT_T[]);
  private:
    DISPLAY_FORMAT_T* _supportedFormats;
    int _set_supported_formats(DISPLAY_FORMAT_T[]);
    

};
#endif