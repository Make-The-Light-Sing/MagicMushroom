#include <TimerOne.h>
#include <Wire.h>
#include <FastSPI_LED2.h>
#include "Color.h"
#include "MushroomHead.h"

#define I2C_HEADER 1
#define DEBUG

#define NUM_LEDS HEAD_NB_LED

struct CRGB                leds[NUM_LEDS];
TM1809Controller800Mhz<6>  LED;
MushroomHead               head;

#define EFFECT_FIRE        0
#define EFFECT_LIGHTNING   1
#define EFFECT_COLORCHASE  2
#define EFFECT_RAINBOW1    3
#define EFFECT_RAINBOW2    4

#define NB_EFFECTS         5
#define SLAVE              1

byte current_effect = 0;
int  interval       = 0;
int  color          = 0;

void setup()
{
    #ifdef DEBUG
        Serial.begin(9600);           // start serial for output
        Serial.println("Resetting Header!");
    #endif
    
//    Timer1.initialize(10000);
//    Timer1.attachInterrupt( timeredDisplay ); // attach the service routine here

    Wire.begin(SLAVE);
    Wire.onReceive(receiveEvent);

    LED.init();
    memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
    LED.showRGB((byte*)leds, NUM_LEDS);

    head.setPixels(leds);
    delay(20);
}

void loop()
{
    switch(current_effect)
    {
        case EFFECT_FIRE : // fire(); break;
        case EFFECT_LIGHTNING : lightning(); break;
        case EFFECT_COLORCHASE : // colorChase(); break;
        case EFFECT_RAINBOW1 : rainbowCycle1(); break;
        case EFFECT_RAINBOW2 : rainbowCycle2(); break;
    }
    
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    current_effect = Wire.read(); // receive byte as a character
    interval       = Wire.read();
    color          = Wire.read();
}

/**
 * Get color from analog inputs
 */
struct CRGB getColor()
{
    return Wheel(color);
}

int getInterval()
{
    return interval;
}

/**
 * Fast light up one strip of the foot
 */
void lightning()
{
    byte x;
    head.turnOff();
    while(current_effect == EFFECT_LIGHTNING)
    {
        x = random(0, HEAD_NB_ROW);
        for(int y=0; y < head.getRowLength(x); y++)
        {
            head.setPixelColor(x, y, getColor());
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(5);
        for(int y=0; y < head.getRowLength(x); y++)
        {
            head.setPixelColor(x, y, Color(0, 0, 0));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(random(0, 100));
    }
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
           for(x =0; x < HEAD_NB_ROW; x++)
//        for (i=0; i < FOOT_STRIP_LENGTH; i++)
        {
            // tricky math! we use each pixel as a fraction of the full 384-color
            // wheel (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 384 is to make the wheel cycle around
//            foot.setPixelColor(i, Wheel(((i * 384 / NUM_LEDS) + j) % 384));
        for (i=0; i < head.getRowLength(x); i++)
//           for(x =0; x < FOOT_NB_STRIP; x++)
           {
               head.setPixelColor(x, i, Wheel(((i * 384 / head.getRowLength(x)) + j) % 384));
           } 
//            foot.setPixelColor(i, Wheel((i * 10 + j * 8) % 384));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(getInterval() / 4);
        
        // If effect have changed, then exit
        if (current_effect != EFFECT_RAINBOW1)
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
            head.setPixelColor(i, Wheel((i * 10 + j * 8) % 384));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(getInterval() / 4);
        
        // If effect have changed, then exit
        if (current_effect != EFFECT_RAINBOW2)
        {
            return;
        }
    }
}
