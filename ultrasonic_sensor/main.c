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
    
    // Turn off outputs at start
    RC0 = false;
    RC3 = false;
    RA4 = false;
    
    __delay_ms(2000); // A little bit of a boot delay
    
    while (1) {
        // sensor1 must be triggered, but not sensor2
        if (check_sensor1() && !check_sensor2()) {
            RC0 = true;
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
    // RC_3 = Ultrasonic Sensor 1 - Trigger (Output)
    // RC_4 = Ultrasonic Sensor 1 - Echo (Input)
    // RA_3 = Ultrasonic Sensor 2 - Trigger (Output)
    // RA_4 = Ultrasonic Sensor 2 - Echo (Input)
    
    RC0_TRIS = OUTPUT;
    
    RC3_TRIS = OUTPUT;
    RC4_TRIS = INPUT;
    
    RA4_TRIS = OUTPUT;
    RA5_TRIS = INPUT;
}


/*
 * Not the most space efficient duping code, but since we have different structs
 * for the inputs / outputs, it is much easier.
 */ 
bool check_sensor1() {
    // Variable to keep track of time
    int time = 0;

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

    return (time * DIST_CONV <= RANGE);
}

bool check_sensor2() {
    // Variable to keep track of time
    int time = 0;

    // Do a quick pulse on the trigger pin of the ultrasonic sensor
    RA4 = true;
    __delay_us(50);
    RA4 = false;

    while (RA5 == false) {
        __delay_us(1);
    }

    while (RA5 == true) {
        time++;
        __delay_us(1);
    }

    return (time * DIST_CONV <= RANGE);
}