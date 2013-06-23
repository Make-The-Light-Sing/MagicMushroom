#ifndef EXTERNALCONTROL_H
#define EXTERNALCONTROL_H

#define PIN_EFFECT         A0

class ExternalControl {
    public:
        ExternalControl() { };
        void initialize(byte n, byte s)
        {
            nb_effects = n;
            slave      = s;
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
        void transmitData()
        {
            Wire.beginTransmission(slave);
            Wire.write(effect);
            Wire.endTransmission();
        }
    private:
        byte slave;
        byte nb_effects;
        byte effect;
};

#endif
