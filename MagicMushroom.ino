// #define FAST_SPI_INTERRUPTS_WRITE_PINS 1
// #define FORCE_SOFTWARE_SPI 1
#include "FastSPI_LED2.h"

// #define DEBUG
#ifdef DEBUG
#define DPRINT Serial.print
#define DPRINTLN Serial.println
#else
#define DPRINT(x)
#define DPRINTLN(x)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// test code
//
//////////////////////////////////////////////////

#define NUM_LEDS 160
struct CRGB { byte g; byte r; byte b; };
struct CRGB leds[NUM_LEDS];
TM1809Controller800Mhz<6> LED;

/**
 * Initialisation
 */
void setup()
{
    LED.init();
}

int count = 0;
long start = millis();

void loop()
{ 
#if 0
    memset(leds, 255, NUM_LEDS * sizeof(struct CRGB));
    LED.showRGB((byte*)leds, NUM_LEDS);
    delay(20);
#else
    for(int i = 0; i < 3; i++)
    {
        for(int iLed = 0; iLed < NUM_LEDS; iLed++)
        {
            memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
            switch(i)
            { 
                case 0: leds[iLed].r = 128; break;
                case 1: leds[iLed].g = 128; break;
                case 2: leds[iLed].b = 128; break;
            }
            if(count == 0)
            { 
		start = millis();
            } 
            if(count++ == 1000)
            { 
		count = 0;
            }
            LED.showRGB((byte*)leds, NUM_LEDS);;
        }
    }
#endif
}
