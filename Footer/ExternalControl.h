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
                transmitData('E', (int) effect);
            }
            return effect;
        };
        int getInterval()
        {
            int new_interval = analogRead(PIN_INTERVAL);
            if (new_interval != interval)
            {
                interval = new_interval;
                transmitData('I', (int) interval);
            }
            return interval;
        };
        struct CRGB getColor()
        {
            int new_color = ((int) (analogRead(PIN_COLOR) / 2.66)) % 384;
            if (new_color != color)
            {
                color = new_color;
                transmitData('C', color);
            }
            return Wheel(color);
        };
        void transmitData(char c, int value)
        {
            int mask = 0xFF;
            byte toSend;
            Wire.beginTransmission(slave);
            Wire.write(c);
            // sent int in 2 bytes
            Wire.write(value & mask);
            Wire.write(value >> 8);
            Wire.endTransmission();
        };
    private:
        byte slave;
        byte nb_effects;
        byte effect;
        int  interval;
        int  color;
};

#endif
