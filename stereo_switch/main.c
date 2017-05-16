/*
 * File:   main.c
 * Author: Andrew McOlash
 *
 * Created on December 25, 2016, 7:54 PM
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------

void init();
unsigned int read_adc();

// MAIN FUNCTION ---------------------------------------------------------------

void main() {
    init();

    // used to store ADC result after capture
    unsigned int value;
    
    while(1) {
      value = read_adc();
      RA1 = value > 512;
      __delay_ms(RA1 * 500 + 500);
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
    
    // Set up analog for only RA_0
    ANSELAbits.ANSA0 = true;
    ANSELAbits.ANSA1 = false;
    ANSELAbits.ANSA2 = false;
    ANSELAbits.ANSA4 = false;
    
    // This selects which analog input to use for the ADC conversion
    ADCON0bits.CHS=0x00;	
    // ADC is on
    ADCON0bits.ADON=1;		
  	// select ADC conversion clock select as Fosc/8
    ADCON1bits.ADCS=0x01;
    // results are right justified
    ADCON1bits.ADFM=0x01;       

    // RA_0 = ADC Input
    // RA_1 = Output
    // RA_2 = Ultrasonic Sensor 1 - Echo (Input)

    // Inputs
    RA0_TRIS = INPUT;
    RA2_TRIS = INPUT;
    
    // Output
    RA1_TRIS = OUTPUT;
}

// This subroutine does the ADC conversion and returns the 10 bit result
unsigned int read_adc() {
    unsigned int ADCValue;
    
    // start conversion
    ADCON0bits.GO = 1;
    
    // wait for conversion to finish
    while (ADCON0bits.GO);
    
    // get the 2 msbs of the result and rotate 8 bits to the left
    ADCValue = ADRESH << 8;
    
    // now add the low 8 bits of the resut into our return variable
    ADCValue = ADCValue + ADRESL;
    
    // return the 10bit result in a single variable
    return ADCValue;
}
