#ifndef EXTERNALCONTROL_H
#define EXTERNALCONTROL_H

#include "Color.h"

#define PIN_EFFECT         A0
#define PIN_COLOR          A1
#define PIN_LIGHTNESS      A2
#define PIN_INTERVAL       A3

class ExternalControl {
    public:
        ExternalControl() { };
        void initialize(byte n, byte s)
        {
            nb_effects = n;
            slave      = s;
            pinMode(PIN_EFFECT, INPUT);
            pinMode(PIN_COLOR, INPUT);
            pinMode(PIN_INTERVAL, INPUT);
        };
        byte getEffect()
        {
            byte new_effect = (analogRead(PIN_EFFECT) / (1024 / nb_effects)) % nb_effects;
            if (new_effect != effect)
            {
                effect = new_effect;
                transmitData();
            }
            return effect;
        };
        int getInterval()
        {
            int new_interval = analogRead(PIN_INTERVAL);
            if (new_interval != interval)
            {
                interval = new_interval;
                transmitData();
            }
            return interval;
        };
        struct CRGB getColor()
        {
            int new_color = ((int) (analogRead(PIN_COLOR) / 2.66)) % 384;
            if (new_color != color)
            {
                color = new_color;
                transmitData();
            }
            return Wheel(color);
        }
    private:
        byte slave;
        byte nb_effects;
        byte effect;
        int  interval;
        int  color;
        void transmitData()
        {
            Wire.beginTransmission(slave);
            Wire.write(effect);
            Wire.write(interval);
            Wire.write(color);
            Wire.endTransmission();
        };
};

#endif
