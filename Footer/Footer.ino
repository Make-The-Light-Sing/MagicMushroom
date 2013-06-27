// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include <Wire.h>
#include <FastSPI_LED2.h>
#include "Color.h"
#include "MushroomFoot.h"
#include "ExternalControl.h"

// #define DEBUG
#define I2C_HEADER         1

#define NUM_LEDS           FOOT_NB_STRIP * FOOT_STRIP_LENGTH

#define EFFECT_FIRE        0
#define EFFECT_LIGHTNING   1
#define EFFECT_RAINBOW1    2
#define EFFECT_RAINBOW2    3
#define EFFECT_FALLING     4
#define EFFECT_WAVE        5
#define EFFECT_COLORCHASE  6

#define NB_EFFECTS         6
#define SLAVE              1

struct CRGB                leds[NUM_LEDS];
TM1809Controller800Mhz<6>  LED;
MushroomFoot               foot;
ExternalControl            ext;

/**
 * Initialisation
 */
void setup()
{
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("resetting!");
    #endif
    
    Wire.begin();
    
    LED.init();
    memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
    LED.showRGB((byte*)leds, NUM_LEDS);
    foot.setPixels(leds);
    
    ext.initialize(NB_EFFECTS, SLAVE);
    
    delay(20);
}

/**
 * The main loop
 */
void loop()
{ 
    switch(ext.getEffect())
    {
        case EFFECT_FIRE       : fire();          break;
        case EFFECT_LIGHTNING  : lightning();     break;
//        case EFFECT_COLORCHASE : colorChase();    break;
        case EFFECT_RAINBOW1   : rainbowCycle1(); break;
        case EFFECT_RAINBOW2   : rainbowCycle2(); break;
        case EFFECT_FALLING    : falling();       break;
        case EFFECT_WAVE       : wave();          break;
    }
}   // loop()

/**
 * Burn the strips of the mushroom's foot
 */
void fire()
{
    byte fire[FOOT_NB_STRIP][FOOT_STRIP_LENGTH];
    memset(fire, 0, NUM_LEDS);
    while(ext.getEffect() == EFFECT_FIRE)
    {
        for(int x = 0; x < FOOT_NB_STRIP; x++)
        {
            fire[x][0] = random(255);
            
            for(int y = (FOOT_STRIP_LENGTH - 1); y > 0; y--)
            {
                fire[x][y] = ((fire[x][y - 1] + fire[x][(y - 2) % FOOT_STRIP_LENGTH]) * (FOOT_STRIP_LENGTH / 2)) / (FOOT_STRIP_LENGTH * 1.045);
            }
            for(int y=0; y < FOOT_STRIP_LENGTH; y++)
            {
                foot.setPixelColor(x, y, getFireColorFromPalette(fire[x][y]));
            }
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
    }
}

/**
 * Fast light up one strip of the foot
 */
void lightning()
{
    byte x;
    foot.turnOff();
    while(ext.getEffect() == EFFECT_LIGHTNING)
    {
        x = random(0, FOOT_NB_STRIP);
        for(int y=0; y < FOOT_STRIP_LENGTH; y++)
        {
//            foot.setPixelColor(x, y, Color(255, 255, 255));
            foot.setPixelColor(x, y, ext.getColor());
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(5);
        for(int y=0; y < FOOT_STRIP_LENGTH; y++)
        {
            foot.setPixelColor(x, y, Color(0, 0, 0));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(random(0, 100));
    }
}

/**
 * Chase one dot along the strips
 */
void colorChase()
{
    uint16_t i;
    foot.turnOff();
    for (i=0; i < NUM_LEDS; i++)
    {
        foot.setPixelColor(i, ext.getColor()); // set one pixel
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(ext.getInterval() / 4);
        foot.setPixelColor(i, Color(0, 0, 0)); // erase pixel (but don't refresh yet)
        
        // If effect have changed, then exit
        if (ext.getEffect() != EFFECT_COLORCHASE)
        {
            LED.showRGB((byte*)leds, NUM_LEDS);
            return;
        }
    }
    LED.showRGB((byte*)leds, NUM_LEDS);
}

/**
 * Rainbow with each strip synchronised
 */
void rainbowCycle1()
{
    uint16_t i, j;
    byte x;
    for (j=0; j < 384; j++)
    {     // 5 cycles of all 384 colors in the wheel
        for (i=0; i < FOOT_STRIP_LENGTH; i++)
        {
            // tricky math! we use each pixel as a fraction of the full 384-color
            // wheel (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 384 is to make the wheel cycle around
//            foot.setPixelColor(i, Wheel(((i * 384 / NUM_LEDS) + j) % 384));
           for(x =0; x < FOOT_NB_STRIP; x++)
           {
               foot.setPixelColor(x, i, Wheel(((i * 384 / FOOT_STRIP_LENGTH) + j) % 384));
           } 
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(ext.getInterval() / 4);
        
        // If effect have changed, then exit
        if (ext.getEffect() != EFFECT_RAINBOW1)
        {
            return;
        }
    }
}

void rainbowCycle2()
{
    uint16_t i, j;
    byte x;
    for (j=0; j < 384; j++)
    {     // 5 cycles of all 384 colors in the wheel
        for (i=0; i < NUM_LEDS; i++)
        {
            // tricky math! we use each pixel as a fraction of the full 384-color
            // wheel (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 384 is to make the wheel cycle around
//            foot.setPixelColor(i, Wheel(((i * 384 / NUM_LEDS) + j) % 384));
            foot.setPixelColor(i, Wheel((i * 10 + j * 8) % 384));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(ext.getInterval() / 4);
        
        // If effect have changed, then exit
        if (ext.getEffect() != EFFECT_RAINBOW2)
        {
            return;
        }
    }
}

void falling()
{
    uint16_t x, y, i;
    foot.turnOff();
    for(i = 0; i < FOOT_STRIP_LENGTH; i++)
    {
        for(y = FOOT_STRIP_LENGTH - 1; y > i; y--)
        {
            for(x =0; x < FOOT_NB_STRIP; x++)
            {
                foot.setPixelColor(x, y, ext.getColor());
            }
            LED.showRGB((byte*)leds, NUM_LEDS);
            delay(ext.getInterval() / 4);
            for(x =0; x < FOOT_NB_STRIP; x++)
            {
                foot.setPixelColor(x, y, Color(0, 0, 0));
            }
            if (ext.getEffect() != EFFECT_FALLING)
            {
                return;
            }
        }
        for(x =0; x < FOOT_NB_STRIP; x++)
        {
            foot.setPixelColor(x, i, ext.getColor());
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
    }
}

void wave()
{
    byte r, g, b, y;
    byte cycle = 8;
    struct CRGB color;

    for(int j = 0; j < (cycle * 4); j++)
    {
        color = ext.getColor();
        for(int x = 0; x < NUM_LEDS; x++)
        {
            y = (x + j) & 0x1f;
            if (y >= (2 * cycle))
            {
                if (y >= (3 * cycle))
                {
                    r = 255 * ((4 * cycle) - y) / cycle;
                    g = 255 * ((4 * cycle) - y) / cycle;
                    b = 255 * ((4 * cycle) - y) / cycle;
                }
                else
                {
                    r = 255 * (y - (2 * cycle)) / cycle;
                    g = 255 * (y - (2 * cycle)) / cycle;
                    b = 255 * (y - (2 * cycle)) / cycle;
                }
            } else {
                if (y >= cycle)
                {
                    r = color.r * ((2 * cycle) - y) / cycle;
                    g = color.g * ((2 * cycle) - y) / cycle;
                    b = color.b * ((2 * cycle) - y) / cycle;
                }
                else
                {
                    r = color.r * y / cycle;
                    g = color.g * y / cycle;
                    b = color.b * y / cycle;
                }
            }
            foot.setPixelColor(x, Color(r, g, b));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        // If effect have changed, then exit
        if (ext.getEffect() != EFFECT_WAVE)
        {
            return;
        }
    }
}    // function wave
