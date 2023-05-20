#include "DEBUGSerialUIDisplay.h"
SUPPORTED_FORMATS DEBUGSerialUIDisplay::DISPLAY_FORMATS =
    {
        // OPTION_BOOLEAN,
        LIST_OPTIONS_SIMPLE,
        // LIST_OPTIONS_INDEXED,
        KEY_VALUE_LIST_SIMPLE,
        // KEY_VALUE_LIST_INDEXED,
        // KEY_OPTION_SCROLL,
        // KEY_OPTION_GRANULAR,
        OPTION_BUTTONS,
        // HEADING_LIST
};

// LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();
DEBUGSerialUIDisplay::DEBUGSerialUIDisplay(){

};

DEBUGSerialUIDisplay::DEBUGSerialUIDisplay(SerialUART *serial)
{
  screen = serial;
};

int DEBUGSerialUIDisplay::init()
{
  return 0;
};

void DEBUGSerialUIDisplay::clear(){};
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

void DEBUGSerialUIDisplay::center_output(int row, std::string *output)
{

  screen->println(output->c_str());
}
void DEBUGSerialUIDisplay::center_output_with_arrows(int row, std::string *output, bool returnArrow, bool nextArrow)
{
  std::string cust_output = "";
  if (returnArrow)
  {
    cust_output.append("<<");
  }
  cust_output.append(output->c_str());
  if (nextArrow)
  {
    cust_output.append(">>");
  };

  center_output(row, &cust_output);
}

void DEBUGSerialUIDisplay::safe_output(char *data)
{

  screen->println(data);
}

void DEBUGSerialUIDisplay::safe_output(char const *data) { safe_output((char *)data); };

void DEBUGSerialUIDisplay::output_auto(ProgramReturn *programOutput)
{
  if (programOutput->formatOfData == U_DEF)
  {
    OUTPUT_FORMAT MATCHED = U_DEF;
    if (programOutput->FORMAT_PREFERENCE == nullptr)
    {
      return;
    }
    else
    {
      auto programItter = programOutput->FORMAT_PREFERENCE->begin();
      auto endItter = programOutput->FORMAT_PREFERENCE->end();
      while (programItter != endItter)
      {
        auto beginDisplay = DISPLAY_FORMATS.begin();
        auto endDisplay = DISPLAY_FORMATS.end();
        while (beginDisplay != endDisplay)
        {
          if (*beginDisplay == *programItter)
          {
            MATCHED = *beginDisplay;
            beginDisplay = DISPLAY_FORMATS.end();
          }
          else
          {
            beginDisplay++;
          }
        };
        if (MATCHED == U_DEF)
        {
          programItter++;
        }
        else
        {
          programItter = endItter;
        }
      };
    }
    if (MATCHED != U_DEF)
    {
    }
    else
    {
      MATCHED = OUTPUT_FORMAT::BASE;
    };
    programOutput->formatOfData = MATCHED;
  }
  else
  {
    output(programOutput);
  }
};

void DEBUGSerialUIDisplay::output(ProgramReturn *programOutput)
{
  if (programOutput->formatOfData == LIST_OPTIONS_SIMPLE)
  {
    display_as_simple_list(programOutput);
  }
  else if (programOutput->formatOfData == KEY_VALUE_LIST_SIMPLE)
  {
    display_as_key_value_list(programOutput);
  }
  else if (programOutput->formatOfData == OPTION_BUTTONS)
  {
    display_as_option_buttons(programOutput);
  };
}

void DEBUGSerialUIDisplay::display_as_option_buttons(ProgramReturn *programOutput)
{
  std::string static_ID = "NO static_ID";
  std::string MESSAGE = std::string();
  OPTION_BUTTONS_STRUCT *data_ptr = (OPTION_BUTTONS_STRUCT *)programOutput->data;
  MESSAGE = data_ptr->message;
  center_output(0, &MESSAGE);
  MESSAGE = data_ptr->buttons.RETURN.print();
  MESSAGE.append("    ");
  MESSAGE.append(data_ptr->buttons.SELECT.print());
  MESSAGE.append("    ");
  MESSAGE.append(data_ptr->buttons.NEXT.print());
  center_output(1, &MESSAGE);
  // screen.print(data_ptr->buttons.NEXT.print().c_str());

  // data_ptr->message
}

void DEBUGSerialUIDisplay::display_as_key_value_list(ProgramReturn *programOutput)
{
  int *index;
  std::string static_ID = "NO static_ID";
  std::string ERROR = "LIST EMPTY";
  if (&programOutput->PROGRAM_ID != nullptr)
  {
    static_ID = programOutput->PROGRAM_ID;
  };
  KEY_VALUE_LIST_SIMPLE_STRUCT *strutDe = (KEY_VALUE_LIST_SIMPLE_STRUCT *)programOutput->data;
  index = (strutDe->INDEX);
  if (strutDe->OPTIONS_MAP.size() < 1)
  {
    center_output_with_arrows(0, &static_ID, false, false);
    center_output_with_arrows(1, &ERROR, false, false);
  }
  else
  {
    if (*(index) < 0)
    {
      *(index) = 0;
    }
    else if (*index >= (int)(strutDe->as_vector().size()))
    {
      *index = strutDe->as_vector().size() - 1;
    };

    bool begginingArrow = true;
    bool endArrow = true;
    if (*index == 0)
    {
      begginingArrow = false;
    };
    if (*(unsigned int *)index == strutDe->as_vector().size() - 1)
    {
      endArrow = false;
    };

    center_output_with_arrows(0, &strutDe->OPTIONS_VECTOR.at(*index).first, begginingArrow, endArrow);
    center_output_with_arrows(1, &strutDe->OPTIONS_VECTOR.at(*index).second, false, false);
  }
}

void DEBUGSerialUIDisplay::display_as_simple_list(ProgramReturn *programOutput)
{

  int *index;
  std::string static_ID = "NO static_ID";
  std::string ERROR = "LIST EMPTY";
  if (&programOutput->PROGRAM_ID != nullptr)
  {
    static_ID = programOutput->PROGRAM_ID;
  };
  LIST_OPTIONS_SIMPLE_STRUCT *strutDe = (LIST_OPTIONS_SIMPLE_STRUCT *)programOutput->data;
  index = (strutDe->INDEX);
  if (strutDe->OPTIONS_VECTOR.size() < 1)
  {
    center_output_with_arrows(0, &static_ID, false, false);
    center_output_with_arrows(1, &ERROR, false, false);
  }
  else
  {
    if (*(index) < 0)
    {
      *(index) = 0;
    }
    else if (*index >= (int)(strutDe->OPTIONS_VECTOR.size()))
    {
      *index = strutDe->OPTIONS_VECTOR.size() - 1;
    };

    bool begginingArrow = true;
    bool endArrow = true;
    if (*index == 0)
    {
      begginingArrow = false;
    };
    if (*(unsigned int *)index == strutDe->OPTIONS_VECTOR.size() - 1)
    {
      endArrow = false;
    };
    screen->println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    screen->println("");
    screen->println("");
    center_output_with_arrows(0, &(strutDe->OPTIONS_VECTOR.at(*index)), begginingArrow, endArrow);
    screen->println("");
    screen->println("");
    screen->println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
  }
}
