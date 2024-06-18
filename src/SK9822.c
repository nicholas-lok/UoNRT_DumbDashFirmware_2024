/*******************************************************************************
 * @File:   SK9822.c
 * @Company: University of Nottingham Racing Team
 * @Author: Nicholas Lok
 * 
 * @Summary: This file contains the "main" function for a project.
 * @Date: June 17, 2024, 12:54 AM
 ******************************************************************************/

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include "../src/SK9822.h"

/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
rgb_colour hsvToRgb( uint16_t h, uint8_t s, uint8_t v ) {
    uint8_t f = ( h % 60 ) * 255 / 60;
    uint8_t p = ( 255 - s ) * (uint16_t)v / 255;
    uint8_t q = ( 255 - f * (uint16_t)s / 255 ) * (uint16_t)v / 255;
    uint8_t t = ( 255 - ( 255 - f ) * (uint16_t)s / 255 ) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch ( ( h / 60 ) % 6 ){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    
    rgb_colour colour = { r, g, b };
    
    return colour;
}

/**
* 
* @param colors
* @param count
* @param brightness
*/
void SK9822_Clear() {
    /** Send Start Frame */
    _SK9822_StartFrame();

    /** Write LED Frames for each LED */
    for ( uint16_t i = 0; i < numOfLEDs; i++ ) {
      _SK9822_ColourFrames( 0, 0, 0, 0 );
    }

    /** Send End Frame */
    _SK9822_EndFrame( numOfLEDs );
}

/**
* 
* @param colors
* @param count
* @param brightness
*/
void SK9822_Write( rgb_colour *colours, uint16_t numOfLEDs, uint8_t brightness ) {
    /** Send Start Frame */
    _SK9822_StartFrame();

    /** Write LED Frames for each LED */
    for ( uint16_t i = 0; i < numOfLEDs; i++ ) {
      _SK9822_ColourFrames( colours[i].red, colours[i].green, colours[i].blue, brightness );
    }

    /** Send End Frame */
    _SK9822_EndFrame( numOfLEDs );
}

/*! Initialises the I/O lines and sends a "Start Frame" signal to the LED
 *  strip.
 *
 * This is part of the low-level interface provided by this class, which
 * allows you to send LED colors as you are computing them instead of
 * storing them in an array.  To use the low-level interface, first call
 * startFrame(), then call sendColor() some number of times, then call
 * endFrame(). */

/**
 */
void _SK9822_StartFrame() {
    __SK9822_ClearPins();
    __SK9822_WritePins(0);
    __SK9822_WritePins(0);
    __SK9822_WritePins(0);
    __SK9822_WritePins(0);
}

/*! Sends a single 24-bit color and an optional 5-bit brightness value.
 * This is part of the low-level interface described in the startFrame()
 * documentation. */
void _SK9822_ColourFrames( uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness ) {
    __SK9822_WritePins( 0b11100000 | brightness );
    __SK9822_WritePins( blue );
    __SK9822_WritePins( green );
    __SK9822_WritePins( red );
}

/*! Sends an "End Frame" signal to the LED strip.  This is the last step in
* updating the LED strip if you are using the low-level interface described
* in the startFrame() documentation.
*
* After this function returns, the clock and data lines will both be
* outputs that are driving low.  This makes it easier to use one clock pin
* to control multiple LED strips. */
void _SK9822_EndFrame( uint16_t numOfLEDs ) {
    /* The data stream seen by the last LED in the chain will be delayed by
     * (count - 1) clock edges, because each LED before it inverts the clock
     * line and delays the data by one clock edge.  Therefore, to make sure
     * the last LED actually receives the data we wrote, the number of extra
     * edges we send at the end of the frame must be at least (count - 1).
     *
     * Assuming we only want to send these edges in groups of size K, the
     * C/C++ expression for the minimum number of groups to send is:
     *
     *   ((count - 1) + (K - 1)) / K
     *
     * The C/C++ expression above is just (count - 1) divided by K,
     * rounded up to the nearest whole number if there is a remainder.
     *
     * We set K to 16 and use the formula above as the number of frame-end
     * bytes to transfer.  Each byte has 16 clock edges.
     *
     * We are ignoring the specification for the end frame in the APA102
     * datasheet, which says to send 0xFF four times, because it does not work
     * when you have 66 LEDs or more, and also it results in unwanted white
     * pixels if you try to update fewer LEDs than are on your LED strip. */
    for ( uint16_t i = 0; i < ( ( numOfLEDs + 14 )/16 ); i++ ) {
      __SK9822_WritePins(0);
    }

    /* We call init() here to make sure we leave the data line driving low
     * even if count is 0 or 1. */
    __SK9822_ClearPins();
}

void __SK9822_ClearPins() {
    MainLED_Data_Clear();
    MainLED_Clk_Clear();
}

void __SK9822_WritePins( uint8_t byte ) {
    ( ( byte >> 7 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Clear();
    MainLED_Clk_Toggle();

    ( ( byte >> 6 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();

    ( ( byte >> 5 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();

    ( ( byte >> 4 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();

    ( ( byte >> 3 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();

    ( ( byte >> 2 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();

    ( ( byte >> 1 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();

    ( ( byte >> 0 ) & 1 ) ? MainLED_Data_Set() : MainLED_Data_Clear();
    MainLED_Clk_Toggle();
    MainLED_Clk_Toggle();
}
/*******************************************************************************
 End of File
 *******************************************************************************/
