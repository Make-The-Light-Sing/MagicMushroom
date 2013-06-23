#include "MushroomHead.h"


/**
 * Simple constructor
 */
MushroomHead::MushroomHead()
{
}    // MushroomHead()

/**
 * Set memory allocated to pixels
 * @param struct CRGB *
 */
void MushroomHead::setPixels(struct CRGB * p)
{
    pixels = p;
}

int MushroomHead::getRowLength(byte row)
{
    switch(row)
    {
        case 0 : return HEAD_ROW0; break;
        case 1 : return HEAD_ROW1; break;
        case 2 : return HEAD_ROW2; break;
        case 3 : return HEAD_ROW3; break;
    }
}


/**
 * Set color of a pixel
 * @param byte        Strip selection
 * @param byte        Position in strip
 * @param struct CRGB color
 */
void MushroomHead::setPixelColor(byte pos_x, byte pos_y, struct CRGB c)
{
    byte x = pos_x % HEAD_NB_ROW;
    byte y = pos_y % getRowLength(x);
    int delta = 0;
    switch(x)
    {
        case 3 : delta += HEAD_ROW3;
        case 2 : delta += HEAD_ROW2;
        case 1 : delta += HEAD_ROW1;
    }
    
    // One strip of two is plug reversed
/*    if (x % 2 == 1)
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
    #endif*/
    
    pixels[delta + y].r = c.r;
    pixels[delta + y].g = c.g;
    pixels[delta + y].b = c.b;
}

/**
 * Set color of a pixel
 * @param int         Index
 * @param struct CRGB color
 */
void MushroomHead::setPixelColor(int index, struct CRGB c)
{
    index = index % HEAD_NB_LED;
    pixels[index].r = c.r;
    pixels[index].g = c.g;
    pixels[index].b = c.b;
}

/**
 * Turn off all leds
 */
void MushroomHead::turnOff()
{
    memset(pixels, 0, HEAD_NB_LED * sizeof(struct CRGB));
}
