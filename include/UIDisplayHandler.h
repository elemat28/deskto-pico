#ifndef UIDSPLHDNLR_H
#define UIDSPLHDNLR_H

enum class DISPLAY_FORMAT_T {
    BASIC_PRINT,
    UPDATEABLE_VALUE,
    SCROLLABLE_LIST,
    SCROLLABLE_VALUES,
    OPTION_BOOLEAN,
    OPTION_BUTTONS,
} ;


class UIDisplayHandler {
  public:
    const static DISPLAY_FORMAT_T DISPLAY_FORMAT;
    virtual int init() = 0;
    virtual void clear() = 0;
    virtual void safe_output(char* data) = 0;
    void safe_output(const char* data);
  protected:
    //DISPLAY_FORMAT_T supportedFormats;
    int declare_supported_formats(DISPLAY_FORMAT_T formatArray[]);
  private:
    DISPLAY_FORMAT_T* _supportedFormats;
    int _set_supported_formats(DISPLAY_FORMAT_T formatArray[]);
    

};
#endif