/* 
 * File:   main.c
 * Author: andrew
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
        __delay_ms(1);
        RC2 = true;
        
        __delay_ms(1);
        RC2 = false;
    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------

void gpio_init() {
    // RC2 is output
    RC2_TRIS = OUTPUT;
}
