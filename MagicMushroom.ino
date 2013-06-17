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

#define EFFECT_FIRE 1


byte current_effect = EFFECT_FIRE;

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
            for(int y=0; y < FOOT_STRIP_LENGTH; y++) {
                foot.setPixelColor(x, y, getFireColorFromPalette(fire[x][y]));
            }
        }
        LED.showRGB((byte*)leds, NUM_LEDS);
        delay(getInterval());
    }
}

