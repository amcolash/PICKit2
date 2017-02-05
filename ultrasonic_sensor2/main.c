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
   
    RA0 = false;
    RA1 = false;
    RA2 = false;
    
    __delay_ms(2000); // A little bit of a boot delay
    
    while (1) {
        // sensor1 must be triggered, but not sensor2
        if (should_trigger()) {
//        if (check_sensor1() || check_sensor2()) {
            RA0 = true;
            __delay_ms((long) RANGE_DELAY);
        } else {
            RA0 = false;
            __delay_ms((long) DELAY);
        }
    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------

void gpio_init() {
    // Disable analog inputs, we want digital!
    ANSELAbits.ANSA0 = false;
    ANSELAbits.ANSA1 = false;
    ANSELAbits.ANSA2 = false;
    ANSELAbits.ANSA4 = false;

    // RA_0 = Output
    // RA_1 = Ultrasonic Sensor 1 - Trigger (Output)
    // RA_2 = Ultrasonic Sensor 2 - Trigger (Output)
    // RA_3 = Ultrasonic Sensor 1 - Echo (Input)
    // RA_4 = Ultrasonic Sensor 2 - Echo (Input)
    
    RA0_TRIS = OUTPUT;
    RA1_TRIS = OUTPUT;
    RA2_TRIS = OUTPUT;
    RA3_TRIS = INPUT;
    RA4_TRIS = INPUT;
    RA5_TRIS = INPUT;
}

bool should_trigger() {
    for (int i = 0; i < 3; i++) {
        if (!check_sensor1() || check_sensor2()) {
            return false;
        }
        
        __delay_ms(250);
    }
    
    return true;
}

/*
 * Not the most space efficient duping code, but since we have different structs
 * for the inputs / outputs, it is much easier.
 */ 
bool check_sensor1() {
    // Variable to keep track of time
    int time = 0;

    // Do a quick pulse on the trigger pin of the ultrasonic sensor
    RA1 = true;
    __delay_us(50);
    RA1 = false;

    while (RA4 == false) {
        __delay_us(1);
    }

    while (RA4 == true) {
        time++;
        __delay_us(1);
    }

    return (time * DIST_CONV <= RANGE);
}

bool check_sensor2() {
    // Variable to keep track of time
    int time = 0;

    // Do a quick pulse on the trigger pin of the ultrasonic sensor
    RA2 = true;
    __delay_us(50);
    RA2 = false;

    while (RA5 == false) {
        __delay_us(1);
    }

    while (RA5 == true) {
        time++;
        __delay_us(1);
    }

    return (time * DIST_CONV <= RANGE);
}