#include "OLEDUIDisplay.h"
SUPPORTED_FORMATS OLEDUIDisplay::DISPLAY_FORMATS =
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
OLEDUIDisplay::OLEDUIDisplay_t OLEDUIDisplay::DEFAULTDISPLAYCONFIG = OLEDUIDisplay_t();

OLEDUIDisplay::OLEDUIDisplay_t::OLEDUIDisplay_t(int I2Caddress, int width, int height)
{
  I2Caddress = I2Caddress;
  width_px = width;
  height_px = height;
};

OLEDUIDisplay::OLEDUIDisplay_t::OLEDUIDisplay_t()
{
  I2Caddress = 0x3C;
  width_px = 128;
  height_px = 64;
};

// LCI2CDisplay_t LCUIDisplay::DEFAULTDISPLAYCONFIG = LCI2CDisplay_t();
OLEDUIDisplay::OLEDUIDisplay() : screen(128, 64, &Wire)
{

  _backlight = false;
  currentLCDConfig = DEFAULTDISPLAYCONFIG;
  DISPLAY_FORMATS = DISPLAY_FORMATS;
};

OLEDUIDisplay::OLEDUIDisplay(TwoWire *wire, u8_t i2c_addr, u8_t width, u8_t height, u8_t rotation)
{
  screen = Adafruit_SSD1306(width, height, wire, -1, 400000UL, 400000UL);
  screen.setRotation(rotation);
  currentLCDConfig = DEFAULTDISPLAYCONFIG;
  DISPLAY_FORMATS = DISPLAY_FORMATS;
};

OLEDUIDisplay::OLEDUIDisplay(OLEDUIDisplay_t displayDefinition) : OLEDUIDisplay(){

                                                                  };

int OLEDUIDisplay::init()
{
  // screen = Adafruit_SSD1306(128, 32, &Wire, 0);
  screen.setTextColor(SSD1306_WHITE);
  screen.setTextSize(1);
  screen.begin(SSD1306_SWITCHCAPVCC, 0x3C, true, false);
  screen.display();
  return 0;
};

void OLEDUIDisplay::clear()
{
  screen.clearDisplay();
  screen.display();
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

void OLEDUIDisplay::center_output(int row, std::string *output)
{
  int16_t x1;
  int16_t y1;
  uint16_t w;
  uint16_t h;
  int16_t current_y = screen.getCursorY();
  // screen.getTextBounds((char *)output, 0, current_y, &x1, &y1, &w, &h);
  String debug = String("");
  screen.getTextBounds((char *)output->c_str(), 0, current_y, &x1, &y1, &w, &h);
  debug = debug + String(x1) + "/" + String(y1) + "/" + String((int16_t)w) + "/" + String(h);

  int16_t begin = (screen.width() - w) >> 1;
  screen.setCursor(begin, current_y + row * 8);
  screen.print(output->c_str());
  // screen.setCursor(0, 0);
  // screen.print(debug.c_str());
  screen.setCursor((begin + w), (current_y + h));
}
void OLEDUIDisplay::center_output_with_arrows(int row, std::string *output, bool returnArrow, bool nextArrow)
{
  // screen.setCursor(0, 0);
  int16_t y_coord = screen.getCursorY();
  center_output(row, output);
  screen.setCursor(screen.getCursorX(), y_coord);
  // screen.print(output->c_str());

  // screen.setCursor((currentLCDConfig.columns - output->length())/2,row);
  // screen.print(output->c_str());
  std::string retButton;
  std::string nextButton;
  retButton = "<<";
  nextButton = ">>";
  int16_t x1;
  int16_t y1;
  uint16_t w;
  uint16_t h;
  screen.getTextBounds(nextButton.c_str(), 0, 0, &x1, &y1, &w, &h);
  int retPos = 0;
  int nextPos = currentLCDConfig.width_px - 24;

  if (output->length() < 11)
  {
  }
  else if (output->length() == 11)
  {
    retButton = "<";
    nextButton = ">>";
  }

  else
  {
    retButton = "<";
    nextButton = ">";
    nextPos = 15;
  };
  if (returnArrow)
  {
    screen.setCursor(retPos, row);
    screen.print(retButton.c_str());
  };
  if (nextArrow)
  {
    screen.setCursor(screen.width() - w, screen.getCursorY());
    screen.print(nextButton.c_str());
  };
  screen.display();
}

void OLEDUIDisplay::safe_output(char *data)
{
  screen.clearDisplay();
  screen.setTextSize(2);
  screen.setCursor(0, 0);

  center_output(0, (std::string *)data);

  screen.invertDisplay(true);
  String debug = String("");
  uint8_t debug_text_size = 1;
  int16_t cursor_y = screen.getCursorY();
  int16_t currentTextSize = cursor_y / 8;
  if (currentTextSize < 1)
  {
    currentTextSize = 1;
  };
  debug += "W:" + String((screen.width() - screen.getCursorX()) << 1);
  debug += " H:" + String(cursor_y);

  screen.setTextSize(debug_text_size);
  screen.setCursor(0, screen.height() - 8 * debug_text_size);
  screen.print(debug.c_str());
  screen.setTextSize(currentTextSize);

  screen.display();
}

void OLEDUIDisplay::safe_output(char const *data) { safe_output((char *)data); };

void OLEDUIDisplay::output_auto(ProgramReturn *programOutput)
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

void OLEDUIDisplay::output(ProgramReturn *programOutput)
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

void OLEDUIDisplay::display_as_option_buttons(ProgramReturn *programOutput)
{
  int16_t x1;
  int16_t y1;
  uint16_t w;
  uint16_t h;
  std::string static_ID = "NO static_ID";
  std::string MESSAGE = std::string();
  OPTION_BUTTONS_STRUCT *data_ptr = (OPTION_BUTTONS_STRUCT *)programOutput->data;
  MESSAGE = data_ptr->message;
  screen.getTextBounds(data_ptr->buttons.SELECT.print().c_str(), 0, 0, &x1, &y1, &w, &h);
  screen.clearDisplay();
  screen.setCursor(0, 0);
  center_output(0, &MESSAGE);
  MESSAGE = data_ptr->buttons.SELECT.print();
  screen.setCursor(0, screen.height() - h);
  center_output(0, &MESSAGE);
  screen.setCursor(0, screen.height() - h);
  screen.print(data_ptr->buttons.RETURN.print().c_str());
  screen.getTextBounds(data_ptr->buttons.NEXT.print().c_str(), 0, 0, &x1, &y1, &w, &h);
  screen.setCursor(screen.width() - w, screen.height() - h);
  screen.print(data_ptr->buttons.NEXT.print().c_str());
  screen.display();

  // data_ptr->message
}

void OLEDUIDisplay::display_as_key_value_list(ProgramReturn *programOutput)
{
  screen.clearDisplay();
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
    // screen.setCursor(0, 0);
    center_output_with_arrows(0, &strutDe->OPTIONS_VECTOR.at(*index).first, begginingArrow, endArrow);

    center_output_with_arrows(1, &strutDe->OPTIONS_VECTOR.at(*index).second, false, false);

    screen.display();
  }
}

void OLEDUIDisplay::display_as_simple_list(ProgramReturn *programOutput)
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
    screen.clearDisplay();
    screen.display();
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

    center_output_with_arrows(0, &(strutDe->OPTIONS_VECTOR.at(*index)), begginingArrow, endArrow);
  }
}
