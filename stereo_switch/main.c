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
//    __delay_ms(5000);
    
    for (int i = 0; i < 100; i++) {
        RA1 = !RA1;
        __delay_ms(50);
    }

    // used to store ADC result after capture
    unsigned int audio_value;
    unsigned int switch_value;

    // Time left before turning off stereo, int so we can make sure it stays >= 0
    int time = 0;
    // Counter to make sure things settle after turning on/off stereo
    int counter = 0;
    int counter_time = COUNTER_TIME;
    
    
    while(true) {
        // Read value of audio input
        audio_value = read_adc(0x02); // RA2
        
        // Indicator LED
        RA1 = audio_value > THRESHOLD;

        if (time <= 0) {
            counter_time--;
            if (RA1) counter++;
            
            if (counter_time < 0) {
                if (counter > COUNTER_VALUE) {
                    time = TRIGGER_TIME * 10;
                }
                
                counter = 0;
                counter_time = COUNTER_TIME;
            }
        } else {
            if (RA1) {
                time = TRIGGER_TIME * 10;
            } else {
                time--;
                if (time < 0) time = 0;
            }
        }
        
        // Turn on/off stereo if counter is set and time/switch combo matches
        if ((switch_value < 512 && time > 0 && RA1) || (switch_value >= 512 && time <= 0 && !RA1)) {
            toggle_power();
            RA1 = false;
        }
        
//        // Increment counter, keep <= 20
//        counter++;
//        if (counter > COUNTER_VALUE) {
//            counter = COUNTER_VALUE;
//        
//            // Set time if audio meets threshold, else decrement time till stereo turns off
//            if (audio_value > THRESHOLD) {
//                time = TRIGGER_TIME * 10;
//            } else {
//                time--;
//                if (time < 0) time = 0;
//            }
//
//            switch_value = read_adc(0x03); // RA4
//            // Turn on/off stereo if counter is set and time/switch combo matches
//            if ((switch_value < 512 && time > 0 && RA1) || (switch_value >= 512 && time <= 0 && !RA1)) {
//                RA1 = false;
//                counter = 0;
//
//                toggle_power();
//            }
//        }
        
        // Turn off adc while waiting, might help with oddities that I have seen
        ADCON0bits.ADON=0;
        __delay_ms(CHECK_DELAY);
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

    // Wait for stereo to turn on/off
//    __delay_ms(10000);
    
    for (int i = 0; i < 300; i++) {
        RA1 = !RA1;
        __delay_ms(50);
    }
    
    // Wait some more since we can't do it all in one call
//    __delay_ms(10000);
}

// This subroutine does the ADC conversion and returns the 10 bit result
unsigned int read_adc(unsigned int channel) {
    // Toggle on ADC
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
