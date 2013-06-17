// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include "FastSPI_LED2.h"
#include "Color.h"
#include "MushroomFoot.h"

// #define DEBUG
#ifdef DEBUG
    #define DPRINT Serial.print
    #define DPRINTLN Serial.println
#else
    #define DPRINT(x)
    #define DPRINTLN(x)
#endif

#define NUM_LEDS 630

struct CRGB               leds[NUM_LEDS];
TM1809Controller800Mhz<6> LED;
MushroomFoot              foot;

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
    for(int x = 0; x < FOOT_NB_STRIP; x++)
    {
        for(int y = 0; y < FOOT_STRIP_LENGTH; y++)
        {
            foot.setPixelColor(x, y, Color(127, 0, 0));
            LED.showRGB((byte*)leds, NUM_LEDS);
            delay(20);
            foot.setPixelColor(x, y, Color(0, 0, 0));
        }
    }
}   // loop()

