#include "UIDisplayHandler.h"

int UIDisplayHandler::_set_supported_formats(DISPLAY_FORMAT_T formatArray[]){
    _supportedFormats = formatArray;
    return 0;
};

int UIDisplayHandler::declare_supported_formats(DISPLAY_FORMAT_T formatArray[]){
    return _set_supported_formats(formatArray);
};

void UIDisplayHandler::safe_output(const char* data){
safe_output((char*)data);
};