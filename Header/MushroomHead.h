#ifndef MUSHROOMHEAD_H
#define MUSHROOMHEAD_H

// Load requirements
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

#include "Color.h"

// Length (in cm) : 740 - 650 - 540 - 410
// Nb Leds        : 222 - 195 - 162 - 123

#define HEAD_ROW0   222
#define HEAD_ROW1   195
#define HEAD_ROW2   162
#define HEAD_ROW3   123

//#define HEAD_NB_LED HEAD_ROW0 + HEAD_ROW1 + HEAD_ROW2 + HEAD_ROW3
#define HEAD_NB_LED 702
#define HEAD_NB_ROW 4

class MushroomHead {
    public:
        MushroomHead();
        void setPixelColor(byte pos_x, byte pos_y, struct CRGB c);
        void setPixelColor(int index, struct CRGB c);
        void setPixels(struct CRGB * p);
        void turnOff();
        int getRowLength(byte row);
    private:
        struct CRGB * pixels;
};

#endif
