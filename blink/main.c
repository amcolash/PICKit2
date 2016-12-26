/* 
 * File:   main.c
 * Author: andrew
 *
 * Created on December 25, 2016, 7:54 PM
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------

void oscillator_init();
void timer_init();
void gpio_init();

// MAIN FUNCTION ---------------------------------------------------------------

void main() {
//    oscillator_init();
    
    gpio_init();

//    timer_init();

//    while (1) {
//        __delay_ms(1000); // Wait, then blink!
//        RC2_LAT ^= 1;     // toggle LED 3
//    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------

void oscillator_init() {
    OSCCONbits.IRCF = 0b1111; // 48 MHz internal clock

    // enable clock tuning from USB
//    ACTCONbits.ACTSRC = 1;
//    ACTCONbits.ACTEN = 1;
}

void timer_init() {
    //Timer2 Registers Prescaler= 4 - TMR2 PostScaler = 12 - PR2 = 250 - Freq = 1000.00 Hz - Period = 0.001000 seconds
    T2CON |= 88; // bits 6-3 Post scaler 1:1 thru 1:16
    T2CONbits.T2CKPS1 = 0; // bits 1-0  Prescaler Rate Select bits
    T2CONbits.T2CKPS0 = 1;
//    PR2 = 250; // PR2 (Timer2 Match value)

//    INTCONbits.GIE = 1; // global interrupts enabled
//    INTCONbits.PEIE = 1; // peripheral interrupts enabled
//    PIE1bits.TMR2IE = 1; // timer 2 interrupt enabled

//    T2CONbits.TMR2ON = 1; // turn timer 2 on
}

void gpio_init() {
    // LEDs as outputs
    RC2_TRIS = OUTPUT;

    RC2 = true;
}
