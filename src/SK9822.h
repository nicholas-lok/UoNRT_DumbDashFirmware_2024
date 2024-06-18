/**************************************************************************//**
 * @file      SK9822.h
 * @brief     This file contains the "main" function for a project.
 * @author    Nicholas Lok
 * @date      17. June 2024
 ******************************************************************************/
#ifndef SK9822_H    /* Guard against multiple inclusion */
#define SK9822_H

/*******************************************************************************
 * Section: Included Files                                                    
 ******************************************************************************/
#include "../src/config/default/peripheral/port/plib_port.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Section: Data Types
 ******************************************************************************/
/**
 *
 */
typedef struct rgb_colour {
  /** Struct Members */
  uint8_t red, green, blue;
} rgb_colour;

/*******************************************************************************
 * Section: Interface Functions
 ******************************************************************************/
/** Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
rgb_colour hsvToRgb( uint16_t h, uint8_t s, uint8_t v );

/** High Level User Functions */

void SK9822_Clear();
void SK9822_Write( rgb_colour *colours, uint16_t numOfLEDs, uint8_t brightness );

/** Mid-Level Protocol Functions */
void _SK9822_StartFrame();
void _SK9822_ColourFrames( uint8_t red, uint8_t green, uint8_t blue, 
                           uint8_t brightness );
void _SK9822_EndFrame( uint16_t numOfLEDs );

/** Low-Level Pin Functions */
/** Clear Data and Clk Lines */
void __SK9822_ClearPins();
void __SK9822_WritePins( uint8_t byte );

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* SK9822_H */

/* *****************************************************************************
 End of File
 */
