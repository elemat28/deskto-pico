#include "UIDisplayHandler.h"




void UIDisplayHandler::safe_output(const char* data){
safe_output((char*)data);
};

void UIDisplayHandler::output_auto(ProgramReturn* programOutput){
    if(programOutput->formatOfData == U_DEF){
        OUTPUT_FORMAT* negotiationResult = negotiate_format(programOutput->FORMAT_PREFERENCE);
        programOutput->formatOfData = *negotiationResult;
        return;
    }
    output(programOutput);
};

OUTPUT_FORMAT* UIDisplayHandler::negotiate_format(SUPPORTED_FORMATS* program){
    SUPPORTED_FORMATS::iterator program_formaters_itter = program->begin();
    while(program_formaters_itter != program->end() ){
        SUPPORTED_FORMATS::const_iterator display_formats_itter = DISPLAY_FORMATS.begin();
        while(display_formats_itter != DISPLAY_FORMATS.end()){
            if(*program_formaters_itter == *display_formats_itter){
                return(&(*program_formaters_itter));
            }
        }
    }
    
    return(&BASE);
}