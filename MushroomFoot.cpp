#include "MushroomFoot.h"


/**
 * Simple constructor
 */
MushroomFoot::MushroomFoot()
{
}    // MushroomFoot()

/**
 * Set memory allocated to pixels
 * @param struct CRGB *
 */
void MushroomFoot::setPixels(struct CRGB * p)
{
    pixels = p;
}


/**
 * Set color of a pixel
 * @param byte        Strip selection
 * @param byte        Position in strip
 * @param struct CRGB color
 */
void MushroomFoot::setPixelColor(byte pos_x, byte pos_y, struct CRGB c)
{
    byte x = pos_x % FOOT_NB_STRIP;
    byte y = pos_y % FOOT_STRIP_LENGTH;
    
    // One strip of two is plug reversed
    if (x % 2 == 1)
    {
        y = FOOT_STRIP_LENGTH - y;
    }
    #ifdef DEBUG
        Serial.print("pos x :");
        Serial.print(pos_x);
        Serial.print("- pos y :");
        Serial.print(pos_y);
        Serial.print(" (x :");
        Serial.print(x);
        Serial.print("- y :");
        Serial.print(y);
        Serial.println(")");
    #endif
    
    pixels[(x * FOOT_STRIP_LENGTH) + y].r = c.r;
    pixels[(x * FOOT_STRIP_LENGTH) + y].g = c.g;
    pixels[(x * FOOT_STRIP_LENGTH) + y].b = c.b;
}

/**
 * Turn off all leds
 */
void MushroomFoot::turnOff()
{
    memset(pixels, 0, FOOT_NB_STRIP * FOOT_STRIP_LENGTH * sizeof(struct CRGB));
}
