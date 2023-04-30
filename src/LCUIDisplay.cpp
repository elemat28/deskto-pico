#include "LCUIDisplay.h"
SUPPORTED_FORMATS LCUIDisplay::DISPLAY_FORMATS = 
{
  //OPTION_BOOLEAN,
  LIST_OPTIONS_SIMPLE,
  //LIST_OPTIONS_INDEXED,
  //KEY_VALUE_LIST_SIMPLE,
  //KEY_VALUE_LIST_INDEXED,
  //KEY_OPTION_SCROLL,
  //KEY_OPTION_GRANULAR,
  //OPTION_BUTTONS,
  //HEADING_LIST
  };
LCUIDisplay::LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();

LCUIDisplay::LCI2CDisplay_t::LCI2CDisplay_t(int I2Caddress, int columns, int lines){
    I2Caddress = I2Caddress;
    columns = columns;
    lines = lines;
};

LCUIDisplay::LCI2CDisplay_t::LCI2CDisplay_t(){
    I2Caddress = 0x27;
    columns = 16;
    lines = 2;
};

//LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();
LCUIDisplay::LCUIDisplay():screen(DEFAULTDISPLAYCONFIG.I2Caddress, DEFAULTDISPLAYCONFIG.columns, DEFAULTDISPLAYCONFIG.lines){
  
 
  _backlight = false;
  currentLCDConfig = DEFAULTDISPLAYCONFIG;
};


LCUIDisplay::LCUIDisplay(LCI2CDisplay_t displayDefinition):LCUIDisplay(){
  
  


};

int LCUIDisplay::init(){
  screen.init();
  screen.backlight();
  return 0;
};

void LCUIDisplay::clear(){
  screen.clear();
};
/*
void LCUIDisplay::output_auto(ProgramReturn* programOutput){
  
  //OUTPUT_FORMAT* negotiate;
  //OUTPUT_FORMAT resolve;
  std::string name = "DUUPA4";
  if(programOutput->formatOfData == U_DEF){
      
    
  }
 
 
  //std::string promo = std::string("");
  
  //promo.append(std::to_string(result));
  safe_output(programOutput->PROGRAM_ID.c_str());
};
*/

void LCUIDisplay::center_output(int row, std::string* output){
  screen.setCursor((currentLCDConfig.columns - output->length())/2,row);
  screen.print(output->c_str());
  
  
}

void LCUIDisplay::safe_output(char* data){
  screen.clear();
  screen.setCursor(0,0);
  screen.print(data);
}


 void LCUIDisplay::output_auto(ProgramReturn* programOtput){
  int* index;
  std::string ID = "NOT_PROCESSED";
  if(&programOtput->PROGRAM_ID != nullptr){
    ID = programOtput->PROGRAM_ID;
  };
  LIST_OPTIONS_SIMPLE_STRUCT* strutDe =(LIST_OPTIONS_SIMPLE_STRUCT*)programOtput->data;
  index = (strutDe->INDEX);
  //ID = (std::to_string(index));
  if(*(index)<0){
    *(index) = 0;
  } else if (*index >= (int)(strutDe->OPTIONS_VECTOR.size()))
  {
    *index = strutDe->OPTIONS_VECTOR.size()-1;
  };
  
  screen.clear();
  //screen.println(strutDe->OPTIONS_VECTOR.at(*index).c_str());
  //screen.print((std::to_string(*index)).c_str());
  center_output(1, &(strutDe->OPTIONS_VECTOR.at(*index)));
};


void LCUIDisplay::output(ProgramReturn* structure){
  std::string str = structure->PROGRAM_ID;
  //std::string loc = std::to_string(structure->formatOfData);
  screen.println(str.c_str());
//safe_output((char*)structure->data);
}
