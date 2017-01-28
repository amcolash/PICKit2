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
    
    while (1) {
        // a little bit of a de-bounce
        __delay_ms(10);
        
        // Toggle LEDs based on button input
        RC2 = (RC4 == true);
        RC3 = (RC4 == false);
    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------

void gpio_init() {
    // RC_2 = LED1
    // RC_3 = LED2
    // RC_4 = Push Button
    
    RC2_TRIS = OUTPUT;
    RC3_TRIS = OUTPUT;
    RC4_TRIS = INPUT;
}