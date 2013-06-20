// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include <TimerOne.h>
#include "FastSPI_LED2.h"
#include "Color.h"
#include "MushroomFoot.h"

// #define DEBUG

#define NUM_LEDS FOOT_NB_STRIP * FOOT_STRIP_LENGTH

struct CRGB               leds[NUM_LEDS];
TM1809Controller800Mhz<6> LED;
MushroomFoot              foot;

#define EFFECT_FIRE        0
#define EFFECT_LIGHTNING   1
#define EFFECT_COLORCHASE  2
#define EFFECT_RAINBOW1    3
#define EFFECT_RAINBOW2    4

#define NB_EFFECTS 5


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
    
    Timer1.initialize(10000);
    Timer1.attachInterrupt( timeredDisplay ); // attach the service routine here
    
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
        case EFFECT_COLORCHASE : colorChase(); break;
        case EFFECT_RAINBOW1 : rainbowCycle1(); break;
        case EFFECT_RAINBOW2 : rainbowCycle2(); break;
    }
}   // loop()

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
    return Color(
        analogRead(A1) / 4,
        analogRead(A2) / 4,
        analogRead(A3) / 4
    );
}

/**
 * Change effect
 */
void timeredDisplay()
{
    current_effect = (analogRead(A0) / (1024 / NB_EFFECTS)) % NB_EFFECTS;
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
    foot.turnOff();
    while(current_effect == EFFECT_LIGHTNING)
    {
        x = random(0, FOOT_NB_STRIP);
        for(int y=0; y < FOOT_STRIP_LENGTH; y++)
        {
            foot.setPixelColor(x, y, getColor());
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
        foot.setPixelColor(i, getColor()); // set one pixel
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(getInterval());
        foot.setPixelColor(i, Color(0, 0, 0)); // erase pixel (but don't refresh yet)
        
        // If effect have changed, then exit
        if (current_effect != EFFECT_COLORCHASE)
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
//        for (i=0; i < NUM_LEDS; i++)
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
//            foot.setPixelColor(i, Wheel((i * 10 + j * 8) % 384));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(getInterval());
        
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
            foot.setPixelColor(i, Wheel((i * 10 + j * 8) % 384));
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(getInterval());
        
        // If effect have changed, then exit
        if (current_effect != EFFECT_RAINBOW2)
        {
            return;
        }
    }
}


