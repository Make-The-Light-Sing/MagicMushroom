// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include "FastSPI_LED2.h"
#include "Color.h"
#include "MushroomFoot.h"

// #define DEBUG

#define NUM_LEDS FOOT_NB_STRIP * FOOT_STRIP_LENGTH

struct CRGB               leds[NUM_LEDS];
TM1809Controller800Mhz<6> LED;
MushroomFoot              foot;

#define EFFECT_FIRE        1
#define EFFECT_LIGHTNING   2
#define EFFECT_COLORCHASE  3


byte current_effect = EFFECT_COLORCHASE;

/**
 * Initialisation
 */
void setup()
{
    #ifdef DEBUG
        Serial.begin(9600);
        Serial.println("resetting!");
    #endif
    
    LED.init();
    memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
    LED.showRGB((byte*)leds, NUM_LEDS);
    
    foot.setPixels(leds);
    delay(20);
}

/**
 * The main loop
 */
void loop()
{ 
    switch(current_effect)
    {
        case EFFECT_FIRE : fire(); break;
        case EFFECT_LIGHTNING : lightning(); break;
        case EFFECT_COLORCHASE : colorChase(Color(0, 0, 127)); break;
    }
}   // loop()

/**
 * Define speed of effects
 */
int getInterval()
{
//    return analogRead(A0) / 4;
    return 0;
}

/**
 * Burn the strips of the mushroom's foot
 */
void fire()
{
    byte fire[FOOT_NB_STRIP][FOOT_STRIP_LENGTH];
    memset(fire, 0, NUM_LEDS);
    while(current_effect == EFFECT_FIRE)
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
        delay(getInterval());
    }
}

/**
 * Fast light up one strip of the foot
 */
void lightning()
{
    byte x;
    while(current_effect == EFFECT_LIGHTNING)
    {
        x = random(0, FOOT_NB_STRIP);
        for(int y=0; y < FOOT_STRIP_LENGTH; y++) {
            foot.setPixelColor(x, y, Color(255, 255, 255));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(5);
        for(int y=0; y < FOOT_STRIP_LENGTH; y++) {
            foot.setPixelColor(x, y, Color(0, 0, 0));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(random(0, 100));
    }
}


// Chase a dot down the strip
// good for testing purposes
void colorChase(struct CRGB color)
{
    // turn off leds :
    memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
    
    for(int x = 0; x < FOOT_NB_STRIP; x++)
    {
        for(int y=0; y < FOOT_STRIP_LENGTH; y++)
        {
            foot.setPixelColor(x, y, color); // set one pixel
            LED.showRGB((byte*)leds, NUM_LEDS);
            delay(getInterval());
            foot.setPixelColor(x, y, Color(0, 0, 0)); // erase pixel (but don't refresh yet)
        }
    }
    LED.showRGB((byte*)leds, NUM_LEDS);
}
