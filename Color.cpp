#include "Color.h"
#include <pgmspace.h>

/**
 * Convert separate R,G,B into combined 32-bit GRB
 * @param  byte r Red color
 * @param  byte g Green color
 * @param  byte b Blue color
 * @return struct CRGB
 */
struct CRGB Color(byte r, byte g, byte b)
{
    struct CRGB led;
    led.r = r;
    led.g = g;
    led.b = b;
    return led;
};

/**
 * Calculate R G B from Hue
 * @param  float v1
 * @param  float v2
 * @param  float vH
 * @return float
 */
float Hue_2_RGB( float v1, float v2, float vH )             //Function Hue_2_RGB
{
    if ( vH < 0 ) 
        vH += 1;
    if ( vH > 1 ) 
        vH -= 1;
    if ( ( 6 * vH ) < 1 ) 
        return ( v1 + ( v2 - v1 ) * 6 * vH );
    if ( ( 2 * vH ) < 1 ) 
        return ( v2 );
    if ( ( 3 * vH ) < 2 ) 
        return ( v1 + ( v2 - v1 ) * (.66-vH) * 6 );
    return ( v1 );
}

/**
 * Calculate RGB color from Hue/Saturation/Lightness
 * @param  float H     Hue from 0 to 1
 * @param  float S     Saturation from 0 to 1
 * @param  float L     Lightness from 0 to 1
 * @return struct CRGB
 */
struct CRGB HSL(float H, float S, float L)
{
    struct CRGB color;
    float var_1;
    float var_2;
    float Hu=H+.33;
    float Hd=H-.33;
    if ( S == 0 )                       //HSL from 0 to 1
    {
        color.r = L * 255;                      //RGB results from 0 to 255
        color.g = L * 255;
        color.b = L * 255;
    }
    else
    {
        if ( L < 0.5 ) 
            var_2 = L * ( 1 + S );
        else
            var_2 = ( L + S ) - ( S * L );
        var_1 = 2 * L - var_2;
        color.r = round(255 * Hue_2_RGB( var_1, var_2, Hu ));
        color.g = round(255 * Hue_2_RGB( var_1, var_2, H ));
        color.b = round(255 * Hue_2_RGB( var_1, var_2, Hd ));
    }
    return color;
}

/**
 * Fire color palete stored in Flash Memory to optiomize SRAM capabilities
 */
PROGMEM prog_uchar fireColorPalette[] = {
    0,0,0,0,4,0,0,8,0,0,12,0,0,16,1,0,20,1,0,24,1,0,28,2,
    0,32,2,0,36,3,0,40,3,0,44,4,0,48,5,0,52,5,0,56,6,0,60,7,
    0,64,8,0,68,9,0,72,10,0,76,11,0,80,13,0,84,14,0,88,15,0,92,17,
    0,96,18,0,100,20,0,104,21,0,108,23,0,112,25,0,116,26,0,120,28,0,124,30,
    0,128,32,0,132,34,0,136,36,0,140,38,0,144,41,0,148,43,0,152,45,0,156,48,
    0,160,50,0,164,53,0,168,55,0,172,58,0,176,61,0,180,64,0,184,66,0,188,69,
    0,192,72,0,196,75,0,200,78,0,204,82,0,208,85,0,212,88,0,216,91,0,220,95,
    0,224,98,0,228,102,0,232,106,0,236,109,0,240,113,0,244,117,0,248,121,0,252,125,
    1,255,128,5,255,132,9,255,136,13,255,140,17,255,144,21,255,148,25,255,151,29,255,155,
    33,255,158,37,255,162,41,255,165,45,255,169,49,255,172,53,255,175,57,255,178,61,255,181,
    65,255,184,69,255,187,73,255,190,77,255,193,81,255,196,85,255,198,89,255,201,93,255,204,
    97,255,206,101,255,208,105,255,211,109,255,213,113,255,215,117,255,218,121,255,220,125,255,222,
    129,255,224,133,255,226,137,255,228,141,255,230,145,255,231,149,255,233,153,255,235,157,255,236,
    161,255,238,165,255,239,169,255,240,173,255,242,177,255,243,181,255,244,185,255,245,189,255,246,
    193,255,247,197,255,248,201,255,249,205,255,250,209,255,251,213,255,252,217,255,252,221,255,253,
    225,255,253,229,255,254,233,255,254,237,255,254,241,255,255,245,255,255,249,255,255,253,255,255,
    255,255,255
};


/**
 * Get fire color from palette
 * @param  byte        x Coordinate in palette
 * @return struct CRGB
 */
struct CRGB getFireColorFromPalette(byte x)
{
    if (x > 128) x = 128;
    return Color(pgm_read_byte_near(fireColorPalette + x * 3 + 1), pgm_read_byte_near(fireColorPalette + x * 3 + 2), pgm_read_byte_near(fireColorPalette + x * 3));
}


