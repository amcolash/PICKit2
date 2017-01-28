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
    
    // Turn off LED at start
    RC0 = false;
    
    while (1) {        
        RC3 = true;
        __delay_us(50);
        RC3 = false;
        
        int time = 0;
        
        while (RC4 == false) {
            __delay_us(1);
        }
        
        while (RC4 == true) {
            time++;
            __delay_us(1);
        }

        RC0 = (time * 1.111) > 30; // Hacked number because of clock speed I set (I think). Anyways, seems to be within 0.25cm at 60cm
        
//        RC0 = (time * 1.7150) > 25; // LED on if more than 25cm away - actual distance was ~ 16cm
        
        __delay_ms(2000);
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