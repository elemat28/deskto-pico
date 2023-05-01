#include "LCUIDisplay.h"
SUPPORTED_FORMATS LCUIDisplay::DISPLAY_FORMATS = 
{
  //OPTION_BOOLEAN,
  LIST_OPTIONS_SIMPLE,
  //LIST_OPTIONS_INDEXED,
  KEY_VALUE_LIST_SIMPLE,
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
  DISPLAY_FORMATS = DISPLAY_FORMATS;
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
  
  int emptyChars = ((currentLCDConfig.columns - output->length())/2);

 
  std::string rowText = std::string();
  rowText.resize(16, ' ');
  
  rowText.replace(emptyChars,output->length() ,*output);
  screen.setCursor(0,row);
  screen.print(rowText.c_str());
  
  
}
void LCUIDisplay::center_output_with_arrows(int row, std::string* output, bool returnArrow, bool nextArrow){
  center_output(row, output);
  //screen.setCursor((currentLCDConfig.columns - output->length())/2,row);
  //screen.print(output->c_str());
  std::string retButton;
  std::string nextButton;
  retButton = "<<";
  nextButton = ">>";
  int retPos = 0;
  int nextPos = currentLCDConfig.columns-2;

  if(output->length()<11){
    
  } else if (output->length()==11)
  {
    retButton = "<";
    nextButton = ">>";
  }
  
  else {
    retButton = "<";
    nextButton = ">";
    nextPos = 15;
    
  };
  if(returnArrow){
    screen.setCursor(retPos, row);
    screen.print(retButton.c_str());
  };
  if(nextArrow){
  screen.setCursor(nextPos, row);
   screen.print(nextButton.c_str());
  };
   
  
}

void LCUIDisplay::safe_output(char* data){
  screen.clear();
  screen.setCursor(0,0);
  screen.print(data);
}


 void LCUIDisplay::output_auto(ProgramReturn* programOutput){
  if(programOutput->formatOfData == U_DEF){
    OUTPUT_FORMAT MATCHED = U_DEF;
    if(programOutput->FORMAT_PREFERENCE == nullptr){
      return;
    } else {
      auto programItter = programOutput->FORMAT_PREFERENCE->begin();
      auto endItter = programOutput->FORMAT_PREFERENCE->end();
      while(programItter != endItter){
        auto beginDisplay = DISPLAY_FORMATS.begin();
      auto endDisplay =  DISPLAY_FORMATS.end();
      while(beginDisplay != endDisplay){
        if(*beginDisplay == *programItter){
          MATCHED = *beginDisplay;
          beginDisplay =  DISPLAY_FORMATS.end();
        } else{beginDisplay++;}
        
      };
        if(MATCHED == U_DEF){programItter++;}
        else {
          programItter = endItter;
        }
      };
      
    }
    if(MATCHED != U_DEF){
  } else {
    MATCHED = OUTPUT_FORMAT::BASE;
  };
  programOutput->formatOfData = MATCHED;
  } else {
    output(programOutput);
  }
};


void LCUIDisplay::output(ProgramReturn* programOutput){
  if(programOutput->formatOfData == LIST_OPTIONS_SIMPLE){
    display_as_simple_list(programOutput);
  } else if (programOutput->formatOfData == KEY_VALUE_LIST_SIMPLE)
  {
    display_as_key_value_list(programOutput);
  }
  
}

void LCUIDisplay::display_as_key_value_list(ProgramReturn* programOutput){
  int* index;
  std::string static_ID = "NO static_ID";
  std::string ERROR = "LIST EMPTY";
  if(&programOutput->PROGRAM_ID != nullptr){
    static_ID = programOutput->PROGRAM_ID;
  };
  KEY_VALUE_LIST_SIMPLE_STRUCT* strutDe =(KEY_VALUE_LIST_SIMPLE_STRUCT*)programOutput->data;
  index = (strutDe->INDEX);
  if(strutDe->OPTIONS_MAP.size()<1){
    center_output_with_arrows(0, &static_ID, false, false);
    center_output_with_arrows(1, &ERROR, false, false);
  
  
  } else{
  if(*(index)<0){
    *(index) = 0;
  } else if (*index >= (int)(strutDe->as_vector().size()))
  {
    *index = strutDe->as_vector().size()-1;
  };
  screen.clear();
  
  bool begginingArrow = true;
  bool endArrow = true;
  if(*index == 0){
    begginingArrow = false;
  };
  if (*index == strutDe->as_vector().size()-1)
  {
    endArrow = false;
  };
  
  center_output_with_arrows(0, &strutDe->OPTIONS_VECTOR.at(*index).first, begginingArrow, endArrow);
  center_output_with_arrows(1, &strutDe->OPTIONS_VECTOR.at(*index).second, false, false);
  }
  }

void LCUIDisplay::display_as_simple_list(ProgramReturn* programOutput){
  
  int* index;
  std::string static_ID = "NO static_ID";
  std::string ERROR = "LIST EMPTY";
  if(&programOutput->PROGRAM_ID != nullptr){
    static_ID = programOutput->PROGRAM_ID;
  };
  LIST_OPTIONS_SIMPLE_STRUCT* strutDe =(LIST_OPTIONS_SIMPLE_STRUCT*)programOutput->data;
  index = (strutDe->INDEX);
  if(strutDe->OPTIONS_VECTOR.size()<1){
    center_output_with_arrows(0, &static_ID, false, false);
    center_output_with_arrows(1, &ERROR, false, false);
  
  
  } else{
  if(*(index)<0){
    *(index) = 0;
  } else if (*index >= (int)(strutDe->OPTIONS_VECTOR.size()))
  {
    *index = strutDe->OPTIONS_VECTOR.size()-1;
  };
  screen.clear();
  
  bool begginingArrow = true;
  bool endArrow = true;
  if(*index == 0){
    begginingArrow = false;
  };
  if (*index == strutDe->OPTIONS_VECTOR.size()-1)
  {
    endArrow = false;
  };

  center_output_with_arrows(0, &(strutDe->OPTIONS_VECTOR.at(*index)), begginingArrow, endArrow);
  }
  }
