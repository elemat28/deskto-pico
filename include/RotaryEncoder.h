#ifndef ROTENPICO_H
#define ROTENPICO_H
#include <string>


class RotaryEncoder
{
private:
    int _value;
public:
    RotaryEncoder();
    ~RotaryEncoder();
    int getValue();
    void turnCW();
    void turnACW();
    void click();

};


#endif