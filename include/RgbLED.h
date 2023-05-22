#ifndef RGBLED_H
#define RGBLED_H
#include <Arduino.h>

class RgbLED
{
public:
    struct RGBColor
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        RGBColor();
        RGBColor(uint8_t red_value,
                 uint8_t green_value,
                 uint8_t blue_value);

        bool operator==(const RGBColor &b);
        bool operator!=(const RGBColor &b);
    };
    struct LEDState
    {
        float brightness;
        RGBColor color;
        LEDState();
        LEDState(RGBColor colorStruct);
        LEDState(RGBColor colorStruct, float newBrightness);
        LEDState(uint8_t red_value,
                 uint8_t green_value,
                 uint8_t blue_value);
        LEDState(uint8_t red_value,
                 uint8_t green_value,
                 uint8_t blue_value, float newBrightness);
        bool operator==(const LEDState &b);
        bool operator!=(const LEDState &b);
    };
    RgbLED();
    RgbLED(uint8_t R_GPIO, uint8_t G_GPIO, uint8_t B_GPIO);
    RgbLED(uint8_t R_GPIO, uint8_t G_GPIO, uint8_t B_GPIO, uint8_t SINK_GPIO);
    int define_pins(uint8_t R_GPIO, uint8_t G_GPIO, uint8_t B_GPIO);
    int set_brightness(float newBrightness);
    float get_brightness();
    int set_color(uint8_t red_value,
                  uint8_t green_value,
                  uint8_t blue_value);
    int set_color(RGBColor colorStruct);
    RGBColor get_color();
    int set_on(bool newState);
    bool is_on();
    int turn_on();
    int turn_off();
    bool toggle();
    LEDState get_state();
    int set_state(LEDState newState);
    int set_state(RGBColor colorStruct, float brightness_value);
    int set_state(uint8_t red_value,
                  uint8_t green_value,
                  uint8_t blue_value,
                  float brightness_value);
    int update_output();

private:
    uint8_t _R_GPIO;
    uint8_t _G_GPIO;
    uint8_t _B_GPIO;
    uint8_t _SINK_GPIO;
    bool _on_state;
    bool _new_on_state;
    LEDState _output_state;
    LEDState _new_state;
};

#endif