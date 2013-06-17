#ifndef MUSHROOMFOOT_H
#define MUSHROOMFOOT_H

// Load requirements
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

#define FOOT_STRIP_LENGTH 90
#define FOOT_NB_STRIP 7

struct CRGB { byte g; byte r; byte b; };

class MushroomFoot {
    public:
        MushroomFoot();
        void setPixelColor(byte pos_x, byte pos_y, struct CRGB c);
        void setPixels(struct CRGB * p);
    private:
        struct CRGB * pixels;
};

#endif
