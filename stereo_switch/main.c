/*
 * File:   main.c
 * Author: Andrew McOlash
 *
 * Created on May 15, 2017, 8:34 PM
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------

void init();
unsigned int read_adc(unsigned int);

// MAIN FUNCTION ---------------------------------------------------------------

void main() {
    init();

    RA0 = false;
    RA1 = false;

    // used to store ADC result after capture
    unsigned int audio_value;
    unsigned int switch_value;

    // int so that we can go negative, and then correct it
    int time = 0;
    // counter to make sure that a few successive checks are valid before committing to turning on
    int counter = 0;

    while(true) {
        // Read value of audio input
        audio_value = read_adc(0x02);

        RA1 = audio_value > THRESHOLD;

        if (audio_value > THRESHOLD) {
            counter += 2;

            if (counter > COUNTER_VALUE) {
                counter = COUNTER_VALUE;
                time = TRIGGER_TIME * 10; // times 10 because we check 10x per second
            }
        } else {
            time--;
            counter--;

            if (time < 0) time = 0; // prevent negative, stick to 0
            if (counter < 0) counter = 0; // prevent negative, stick to 0
        }

        // Check if the stereo is on, then quick flip the switch
        switch_value = read_adc(0x03);
        
        if ((switch_value < 512 && time > 0) || (switch_value > 512 && time == 0)) {
            // Toggle the power switch
            RA0 = true;
            __delay_ms(100);
            RA0 = false;

            // Wait for stereo to turn on/off
            __delay_ms(10000);
        }
        
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

// This subroutine does the ADC conversion and returns the 10 bit result
unsigned int read_adc(unsigned int channel) {
    // This selects which analog input to use for the ADC conversion
    ADCON0bits.CHS = channel;
    
    // Wait for channel switch
    __delay_us(50);
    
    // start conversion
    ADCON0bits.GO = 1;

    // wait for conversion to finish
    while (ADCON0bits.GO);

    // return the 10bit result in a single variable
    // get the 2 msbs of the result and rotate 8 bits to the left
    // then add the low 8 bits of the resut into our return variable
    return (ADRESH << 8) + ADRESL;
}
