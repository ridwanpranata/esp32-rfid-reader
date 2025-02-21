#ifndef LIQUIDCRYSTAL_H
#define LIQUIDCRYSTAL_H

#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

class LiquidCrystal
{
public:
    LiquidCrystal(uint8_t column, uint8_t row);
    void init();
    void setLineText(uint8_t row, const char* text);

private:
    uint8_t _column;
    uint8_t _row;
    LiquidCrystal_I2C _lcd;

    void boatingAnimation();
};

#endif