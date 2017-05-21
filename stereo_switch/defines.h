/*
 * File:   defines.h
 * Author: Andrew McOlash
 *
 * Created on May 15, 2017, 8:36 PM
 */

#ifndef DEFINES_H
#define	DEFINES_H


// INCLUDES --------------------------------------------------------------------

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

// CONFIGURATION BITS ----------------------------------------------------------

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//// CONFIG1
//#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
//#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
//#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
//#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
//#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
//#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
//#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
//#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
//#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
//
//// CONFIG2
//#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
//#pragma config CPUDIV = NOCLKDIV// CPU System Clock Selection Bit (NO CPU system divide)
//#pragma config USBLSCLK = 48MHz // USB Low SPeed Clock Selection bit (System clock expects 48 MHz, FS/LS USB CLKENs divide-by is set to 8.)
//#pragma config PLLMULT = 4x     // PLL Multipler Selection Bit (4x Output Frequency Selected)
//#pragma config PLLEN = DISABLED // PLL Enable Bit (3x or 4x PLL Disabled)
//#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
//#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
//#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
//#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
//
//// DEFINES ---------------------------------------------------------------------


#pragma config FOSC=INTOSC
#pragma config PLLEN=OFF
#pragma config WDTE=OFF
#pragma config MCLRE=OFF
#pragma config CLKOUTEN=OFF
#pragma config IESO=OFF
#pragma config FCMEN=OFF
#pragma config CP=OFF
#pragma config CPD=OFF
#pragma config BOREN=OFF
#pragma config WRT=OFF
#pragma config STVREN=ON
#pragma config BORV=LO
#pragma config LVP=OFF

// this is used by the __delay_ms(xx) and __delay_us(xx) functions
#define _XTAL_FREQ    16000000

// Defines for input / output and booleans
#define OUTPUT        0
#define INPUT         1

typedef int bool;
#define true          1
#define false         0

// Voltage threshold (from 0-1024) to trigger on state
#define THRESHOLD     400

// The value the counter needs to be at before triggering stereo on
#define COUNTER_VALUE 8

// Delays for re-checking if the state has changed
// Delay in ms
#define CHECK_DELAY   100

// Time turned on after silence in sec
#define TRIGGER_TIME  60

// RA_0 = Final Output
// RA_1 = Temp Output
// RA_2 = ADC Input
// RA_3 = Switch Output
// RA_4 - RA_5 = Unused

// GPIO
#define RA0_TRIS      TRISAbits.TRISA0
#define RA0           LATAbits.LATA0

#define RA1_TRIS      TRISAbits.TRISA1
#define RA1           LATAbits.LATA1

#define RA2_TRIS      TRISAbits.TRISA2
#define RA2           ADCON0bits.GO

#define RA3_TRIS      TRISAbits.TRISA3
#define RA3           PORTAbits.RA3

#endif	/* DEFINES_H */
