/*
 * File:   main.c
 * Author: Andrew McOlash
 *
 * Created on May 15, 2017, 8:34 PM
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------

void init();
void toggle_power();
unsigned int read_adc(unsigned int);

// MAIN FUNCTION ---------------------------------------------------------------

void main() {
    init();

    RA0 = false;
    RA1 = false;
    
    // Boot delay
    // Long enough to handle a brown out on power toggle
    for (int i = 0; i < 50; i++) {
        RA1 = !RA1;
        __delay_ms(50);
    }

    // used to store ADC result after capture
    unsigned int audio_value;
    unsigned int led_value;

    // Time left before turning off stereo, int so we can make sure it stays >= 0
    int time = 0;
    // Counter to make sure things settle after turning on/off stereo
    int counter = 0;
    // previous led value
    unsigned int last_led_value = 0;
    // timeout from manual power toggle
    int manual_timer = 0;
    
    while(true) {
        counter = 0;
        
        for (int i = 0; i < SAMPLE_TIME * 10; i++) {
            // Read value of audio input
            audio_value = read_adc(0x02); // RA2

            // Indicator LED
            RA1 = audio_value > THRESHOLD;

            counter += audio_value > THRESHOLD;
            
            // Turn off adc while waiting, might help with oddities that I have seen
            ADCON0bits.ADON=0;
            __delay_ms(CHECK_DELAY);
        }
        
        led_value = read_adc(0x03); //< 512) ? false : true; // RA4 (LED), false if off, true if on
        
        // Handle manual power on / off
        if (last_led_value > 512 && led_value <= 512 && time > 0) {
            // If was on and now is off, turn off power
            time = 0;
            counter = 0;
            manual_timer = TRIGGER_TIME;
        } else if (last_led_value <= 512 && led_value > 512) {
            // If was off and now is on, turn on power
            counter = COUNTER_VALUE + 1;
            manual_timer = TRIGGER_TIME;
        }
        
        // Save last value
        last_led_value = led_value;
        
        // If in manual state, stop here
        if (manual_timer > 0) {
            manual_timer--;
            continue;
        }
        
        // led_value > 512 = LED is on, led_value < 512 = LED is off
        if (led_value > 512) { // LED on
            // Reset time to max if threshold is met
            if (counter >= COUNTER_VALUE) {
                time = TRIGGER_TIME;
            } else {
                // Decrement if threshold not met
                time--;
                
                // If time runs out, turn off
                if (time <= 0) {
                    time = 0;
                    toggle_power();
                }
            }
        } else { // LED off
            // Need to turn on since enough samples we positive
            if (counter >= COUNTER_VALUE) {
                time = TRIGGER_TIME;
                toggle_power();
            }
        }
        
        // Code to test the different states
//        int state = 0;
//        if (led_value > 512) {
//            // Reset time to max if threshold is met
//            if (counter >= COUNTER_VALUE) {
//                state = 1;
//            } else {
//                state = 2;
//            }
//        } else {
//            // Need to turn on since enough samples we positive
//            if (counter >= COUNTER_VALUE) {
//                state = 3;
//            } else {
//                state = 4;
//            }
//        }
//        
//        for (int i = 0; i < state; i++) {
//            RA1 = true;
//            __delay_ms(1000);
//            RA1 = false;
//            __delay_ms(500);
//        }
    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------


// This subroutine sets up our pins and other bits to configure
void init() {
    // set up oscillator control register

    // PLL is disabled
    OSCCONbits.SPLLEN=0;
    //set OSCCON IRCF bits to select OSC frequency=16Mhz
    OSCCONbits.IRCF=0x0F;
    //set the SCS bits to select internal oscillator block
    OSCCONbits.SCS=0x02;
    // OSCON should be 0x7Ah now.

    // turn DAC off
    DACCON0bits.DACEN=0;

    // Set up analog for only RA_2
    ANSELAbits.ANSA0 = false;
    ANSELAbits.ANSA1 = false;
    ANSELAbits.ANSA2 = true;
    ANSELAbits.ANSA4 = true;

    // Turn the ADC on
    ADCON0bits.ADON=1;
  	// select ADC conversion clock select as Fosc/8
    ADCON1bits.ADCS=0x01;
    // results are right justified
    ADCON1bits.ADFM=0x01;

    // RA_0 = Final Output
    // RA_1 = Temp Output
    // RA_2 = Audio Input
    // RA_4 = Switch Output
    // RA_3, RA_5 = Unused

    // Outputs
    RA0_TRIS = OUTPUT;
    RA1_TRIS = OUTPUT;

    // Inputs
    RA2_TRIS = INPUT;
    RA4_TRIS = INPUT;
}

void toggle_power() {
    // Toggle the power switch
    RA0 = true;
    __delay_ms(300);
    RA0 = false;

    // Wait for stereo to turn on/off, need to do in multiple calls
    __delay_ms(10000);
}

// This subroutine does the ADC conversion and returns the 10 bit result
unsigned int read_adc(unsigned int channel) {
    // Toggle off/on ADC
    ADCON0bits.ADON=0;
    __delay_ms(1);
    ADCON0bits.ADON=1;
    __delay_ms(1);
    
    // This selects which analog input to use for the ADC conversion
    ADCON0bits.CHS = channel;
    
    // Wait for channel switch
    __delay_ms(1);
    
    // start conversion
    ADCON0bits.GO = 1;

    // wait for conversion to finish
    while (ADCON0bits.GO);

    // return the 10bit result in a single variable
    // get the 2 msbs of the result and rotate 8 bits to the left
    // then add the low 8 bits of the resut into our return variable
    return (ADRESH << 8) + ADRESL;
}
