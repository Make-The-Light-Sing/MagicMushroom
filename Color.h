#ifndef COLOR_H
#define COLOR_H

// Load requirements
#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

// Define the RGB Pixel structure
struct CRGB { byte b; byte r; byte g; };

struct CRGB 
    Color(byte r, byte g, byte b),
    HSL(float H, float S, float L),
    getFireColorFromPalette(byte x);
float
    Hue_2_RGB( float v1, float v2, float vH );

#endif
