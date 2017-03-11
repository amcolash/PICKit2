/*
 * File:   main.c
 * Author: Andrew McOlash
 *
 * Created on December 25, 2016, 7:54 PM
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------

void gpio_init();
bool check_sensor1();
bool check_sensor2();

// MAIN FUNCTION ---------------------------------------------------------------

void main() {
    gpio_init();

    RA0 = false;
    RA2 = false;
    RA5 = false;

    __delay_ms(2000); // A little bit of a boot delay

    int time = 0;
    bool sensor1 = false;
    bool sensor2 = false;

    while (1) {
        sensor1 = check_sensor1();
        sensor2 = check_sensor2();

        if (sensor1 && !sensor2) {
            time = TRIGGER_TIME * 2;
        } else if (sensor2) {
            time = 0;
        } else {
            time--;
            if (time < 0) time = 0; // prevent underflow
        }

        RA0 = time > 0;
        __delay_ms(450);
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
    // RA_2 = Ultrasonic Sensor 1 - Echo (Input)
    // RA_4 = Ultrasonic Sensor 2 - Echo (Input)
    // RA_5 = Ultrasonic Sensor 2 - Trigger (Output)

    // Output
    RA0_TRIS = OUTPUT;

    // Sensor 1
    RA1_TRIS = OUTPUT;
    RA2_TRIS = INPUT;

    // Unused
//    RA3_TRIS = OUTPUT;

    // Sensor 2
    RA4_TRIS = INPUT;
    RA5_TRIS = OUTPUT;
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

    while (RA2 == false) {
        __delay_us(1);
    }

    while (RA2 == true) {
        time++;
        __delay_us(1);
    }

    return (time * DIST_CONV <= SENSOR1_RANGE);
}

bool check_sensor2() {
    // Variable to keep track of time
    int time = 0;

    // Do a quick pulse on the trigger pin of the ultrasonic sensor
    RA5 = true;
    __delay_us(50);
    RA5 = false;

    while (RA4 == false) {
        __delay_us(1);
    }

    while (RA4 == true) {
        time++;
        __delay_us(1);
    }

    return (time * DIST_CONV <= SENSOR2_RANGE);
}
