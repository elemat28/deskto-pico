#ifndef UIDSPLHDNLR_H
#define UIDSPLHDNLR_H
#include <DeskopicoProgram.h>



class UIDisplayHandler {
  public:
    SUPPORTED_FORMATS DISPLAY_FORMATS;
    virtual int init() = 0;
    virtual void clear() = 0;
    virtual void safe_output(char* data) = 0;
    void safe_output(const char* data);
    void output_auto(ProgramReturn* programOtput);
    
    protected:
    virtual void output(ProgramReturn*programOtput) = 0 ;
    virtual OUTPUT_FORMAT* negotiate_format(SUPPORTED_FORMATS* program);
    private:
    OUTPUT_FORMAT BASE;

};
#endif