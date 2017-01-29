/* 
 * File:   main.c
 * Author: Andrew McOlash
 *
 * Created on December 25, 2016, 7:54 PM
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------

void gpio_init();

// MAIN FUNCTION ---------------------------------------------------------------

void main() {
    gpio_init();
    
    // Variable to keep track of time
    int time = 0;
    
    // Turn off outputs at start
    RC0 = false;
    RC3 = false;
    
    __delay_ms(2000); // A little bit of a boot delay
    
    while (1) {
        // Reset timer
        time = 0;
        
        // Do a quick pulse on the trigger pin of the ultrasonic sensor
        RC3 = true;
        __delay_us(50);
        RC3 = false;
        
        while (RC4 == false) {
            __delay_us(1);
        }
        
        while (RC4 == true) {
            time++;
            __delay_us(1);
        }

        if (time * DIST_CONV <= RANGE) {
            RC0 =  true;
            __delay_ms((long) RANGE_DELAY);
        } else {
            RC0 = false;
            __delay_ms((long) DELAY);
        }
    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------

void gpio_init() {
    // RC_0 = LED1
    // RC_3 = Ultrasonic Sensor - Trigger (Output)
    // RC_4 = Ultrasonic Sensor - Echo (Input)
    
    RC0_TRIS = OUTPUT;
    RC3_TRIS = OUTPUT;
    RC4_TRIS = INPUT;
}