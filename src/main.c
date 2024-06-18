/******************************************************************************* 
 * @File:   main.c
 * @Company: University of Nottingham Racing Team
 * @Author: Nicholas Lok
 * 
 * @Summary: This file contains the "main" function for a project.
 * @Date: June 17, 2024, 12:54 AM
 ******************************************************************************/

/*******************************************************************************
 * Section: Included Files
 ******************************************************************************/
#include <stddef.h>                                                                 // Defines NULL
#include <stdbool.h>                                                                // Defines true
#include <stdlib.h>                                                                 // Defines EXIT_FAILURE
#include "../src/config/default/definitions.h"                                      // SYS function prototypes
#include <time.h>
#include "../src/SK9822.h"

/*******************************************************************************
 * Section: Constants
 ******************************************************************************/
#define LEDCOUNT 28                                                                 // Define Number Of LEDs

/*******************************************************************************
 * Section: Global Variables
 ******************************************************************************/
rgb_colour LED_Colours[LEDCOUNT];                                                   // Create a buffer for holding the colours (3 bytes per colour).

/*******************************************************************************
 * Section: Helper Methods
 ******************************************************************************/
/**
 * 
 * @param s - Delay amount in Seconds
 */
void delay_s( uint32_t s ) {
    struct tm timeStruct;
    RTC_RTCCTimeGet( &timeStruct );
    uint32_t Start = timeStruct.tm_sec;
    
    /** Wait until delay amount is met */
    while ( ( timeStruct.tm_sec - Start ) < s ) {
        RTC_RTCCTimeGet( &timeStruct );
    }
}

/*******************************************************************************
 * Section: Main Entry Point
 ******************************************************************************/
int main ( void ) {
    /* Initialise all modules */
    SYS_Initialize ( NULL );
    
    /** */
    struct tm timeStruct;
    struct tm initialTime = { 0, 0, 0, 0, 0, 0 };
    RTC_RTCCTimeSet( &initialTime );
    
    /** */
    rgb_colour off = { 0, 0, 0 };
    SK9822_Write( off, LEDCOUNT, 1 );
    delay_s( 2 );
    
    /** Main Loop */
    while ( true ) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        /**  */
        RTC_RTCCTimeGet( &timeStruct );
        uint8_t time = timeStruct.tm_sec;
        
        /** */
        for ( uint16_t i = 0; i < LEDCOUNT; i++ ) {
            uint8_t p = time - i * 8;
            LED_Colours[i] = hsvToRgb( ( ( (uint32_t)p * 359 ) / 256 ), 255, 255 );
        }
        
        /** */
        SK9822_Write( LED_Colours, LEDCOUNT, 1 );
        delay_s( 2 );
        
        SK9822_Write( LED_Colours, LEDCOUNT, 10 );
        delay_s( 2 );
    }

    /* Execution should not come here during normal operation */
    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

