#include "RgbLED.h"

RgbLED::RGBColor::RGBColor()
{
    red = 0;
    green = 0;
    blue = 0;
};

RgbLED::RGBColor::RGBColor(uint8_t red_value,
                           uint8_t green_value,
                           uint8_t blue_value)
{
    red = red_value;
    green = green_value;
    blue = blue_value;
}
bool RgbLED::RGBColor::operator==(const RGBColor &b)
{
    if (this->red != b.red)
    {
        return false;
    }
    else if (this->green != b.green)
    {
        return false;
    }
    else if (this->green != b.green)
    {
        return false;
    };
    return true;
};

bool RgbLED::RGBColor::operator!=(const RGBColor &b)
{
    if (*this == b)
    {
        return false;
    }
    else
    {
        return true;
    };
};
RgbLED::LEDState::LEDState()
{
    brightness = 0.0;
    color = RGBColor(255, 255, 255);
};

RgbLED::LEDState::LEDState(RGBColor colorStruct)
{
    brightness = 0.5;
    color = colorStruct;
};

RgbLED::LEDState::LEDState(RGBColor colorStruct, float newBrightness)
{
    brightness = newBrightness;
    color = colorStruct;
};

RgbLED::LEDState::LEDState(uint8_t red_value,
                           uint8_t green_value,
                           uint8_t blue_value)
{
    brightness = 0.5;
    color = RGBColor(red_value, green_value, blue_value);
};

RgbLED::LEDState::LEDState(uint8_t red_value,
                           uint8_t green_value,
                           uint8_t blue_value, float newBrightness)
{
    brightness = newBrightness;
    color = RGBColor(red_value, green_value, blue_value);
};

bool RgbLED::LEDState::operator==(const LEDState &b)
{
    if (this->color != b.color)
    {
        return false;
    }
    else if (this->brightness != b.brightness)
    {
        return false;
    };

    return true;
};

bool RgbLED::LEDState::operator!=(const LEDState &b)
{
    if (*this == b)
    {
        return false;
    }
    else
    {
        return true;
    };
};

RgbLED::RgbLED()
{
    _R_GPIO = 0;
    _G_GPIO = 0;
    _B_GPIO = 0;
    _SINK_GPIO = 0;
    _on_state = false;
    _new_on_state = _on_state;
    _output_state = LEDState();
    _new_state = _output_state;
};

RgbLED::RgbLED(uint8_t R_GPIO, uint8_t G_GPIO, uint8_t B_GPIO) : RgbLED()
{
    _R_GPIO = R_GPIO;
    _G_GPIO = G_GPIO;
    _B_GPIO = B_GPIO;
};

RgbLED::RgbLED(uint8_t R_GPIO, uint8_t G_GPIO, uint8_t B_GPIO, uint8_t SINK_GPIO) : RgbLED(R_GPIO, G_GPIO, B_GPIO)
{
    _R_GPIO = R_GPIO;
    _G_GPIO = G_GPIO;
    _B_GPIO = B_GPIO;
    _SINK_GPIO = SINK_GPIO;
};

int RgbLED::define_pins(uint8_t R_GPIO, uint8_t G_GPIO, uint8_t B_GPIO)
{
    _R_GPIO = R_GPIO;
    _G_GPIO = G_GPIO;
    _B_GPIO = B_GPIO;
    return 0;
};

int RgbLED::set_brightness(float newBrightness)
{
    if (newBrightness > 1)
    {
        _new_state.brightness = 1;
        return 1;
    }
    else if (newBrightness < 0)
    {
        _new_state.brightness = 0;
        return -1;
    }
    else
    {
        _new_state.brightness = newBrightness;
        return 0;
    };
};

float RgbLED::get_brightness()
{
    return _output_state.brightness;
};
int RgbLED::set_color(uint8_t red_value,
                      uint8_t green_value,
                      uint8_t blue_value)
{
    _new_state.color = RGBColor(red_value, green_value, blue_value);
    return 0;
};
int RgbLED::set_color(RGBColor colorStruct)
{
    _new_state.color = colorStruct;
    return 0;
};
RgbLED::RGBColor RgbLED::get_color()
{
    return _output_state.color;
};
int RgbLED::set_on(bool newState)
{
    _new_on_state = newState;
    return 0;
};
bool RgbLED::is_on()
{
    return _on_state;
};

int RgbLED::turn_on()
{
    if (_new_on_state)
    {
        return 1;
    };
    _new_on_state = true;
    return 0;
};
int RgbLED::turn_off()
{
    if (!_new_on_state)
    {
        return 1;
    };
    _new_on_state = false;
    return 0;
};
bool RgbLED::toggle()
{
    if (_on_state)
    {
        turn_off();
    }
    else
    {
        turn_on();
    }
    return _new_on_state;
};

RgbLED::LEDState RgbLED::get_state()
{
    return _output_state;
};

int RgbLED::set_state(LEDState newState)
{
    _new_state = newState;
    return 0;
};

int RgbLED::set_state(RGBColor colorStruct, float brightness_value)
{
    set_color(colorStruct);
    set_brightness(brightness_value);
    return 0;
};
int RgbLED::set_state(uint8_t red_value,
                      uint8_t green_value,
                      uint8_t blue_value,
                      float brightness_value)
{
    set_color(red_value, green_value, blue_value);
    set_brightness(brightness_value);
    return 0;
};
int RgbLED::update_output()
{
    analogWriteRange(255);
    if ((_output_state == _new_state))
    {
        if (_on_state == _new_on_state)
        {
            return 1;
        };
    }
    else
    {
        _output_state = _new_state;
    };
    _on_state = _new_on_state;
    if (_on_state)
    {
        if (_SINK_GPIO != 0)
        {
            pinMode(_SINK_GPIO, INPUT_PULLDOWN);
        };
        analogWrite(_R_GPIO, _output_state.color.red * _output_state.brightness);
        analogWrite(_G_GPIO, _output_state.color.green * _output_state.brightness);
        analogWrite(_B_GPIO, _output_state.color.blue * _output_state.brightness);
    }
    else
    {
        if (_SINK_GPIO != 0)
        {
            pinMode(_SINK_GPIO, INPUT_PULLUP);
        }
        else
        {
            analogWrite(_R_GPIO, 0);
            analogWrite(_G_GPIO, 0);
            analogWrite(_B_GPIO, 0);
        };
    };
    return 0;
};