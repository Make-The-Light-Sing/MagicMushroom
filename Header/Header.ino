#include <Wire.h>
#include "FastSPI_LED2.h"
#include "Color.h"
#include "MushroomHead.h"

#define I2C_HEADER 1
#define DEBUG



#define NUM_LEDS HEAD_NB_LED

struct CRGB               leds[NUM_LEDS];
TM1809Controller800Mhz<6> LED;
MushroomHead              head;

#define EFFECT_FIRE        0
#define EFFECT_LIGHTNING   1
#define EFFECT_COLORCHASE  2
#define EFFECT_RAINBOW1    3
#define EFFECT_RAINBOW2    4

#define NB_EFFECTS 5

byte current_effect = EFFECT_COLORCHASE;
int  interval       = 0;


void setup()
{
    #ifdef DEBUG
        Serial.begin(9600);           // start serial for output
        Serial.println("Resetting Header!");
    #endif
    Wire.begin(I2C_HEADER);                // join i2c bus with address #4
    Wire.onReceive(receiveEvent); // register event

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
        case EFFECT_RAINBOW1 : // rainbowCycle1(); break;
        case EFFECT_RAINBOW2 : rainbowCycle2(); break;
    }    
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    current_effect = Wire.read();    // receive byte as an integer
    interval       = Wire.read();
    #ifdef DEBUG
        Serial.println(current_effect);         // print the integer
    #endif
}

/**
 * Define speed of effects
 */
int getInterval()
{
//    return analogRead(A4) / 4;
    return 0;
}

/**
 * Get color from analog inputs
 */
struct CRGB getColor()
{
    return Color(255, 255, 255);
/*    return Color(
        analogRead(A1) / 4,
        analogRead(A2) / 4,
        analogRead(A3) / 4
    ); */
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


void rainbowCycle2()
{
    uint16_t i, j;
    byte x;
    for (j=0; j < 384; j++)
    {     // 5 cycles of all 384 colors in the wheel
        for (i=0; i < NUM_LEDS; i++)
        {
//            foot.setPixelColor(i, Wheel(((i * 384 / NUM_LEDS) + j) % 384));
            head.setPixelColor(i, Wheel((i * 10 + j * 8) % 384));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
//        delay(getInterval());
        
        // If effect have changed, then exit
        if (current_effect != EFFECT_RAINBOW2)
        {
            return;
        }
    }
}
