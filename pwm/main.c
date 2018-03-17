/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.65
        Device            :  PIC12F1822
        Driver Version    :  2.00
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "eeprom.h"
#include "mcc_generated_files/mcc.h"

const uint8_t MIN = 16;
const uint8_t MAX = 40;

const unsigned char EEPROM_ADDRESS = 0x00;

/*
    Main application
 */
void main(void)
{
    uint8_t angle; // between MIN/MAX
    
    int i;
    int duration;
    bool stage1 = true;
    
    // initialize the device
    SYSTEM_Initialize();
    
    // Wait for things to settle before reading EEPROM
    __delay_ms(100);
    
    // Read the previous boot count from eeprom memoryLocation 0x00
    unsigned char boot_count = EEPROM_ReadByte(EEPROM_ADDRESS);
    
    // Write our new boot count at eeprom memoryLocation 0x00
    EEPROM_WriteByte(EEPROM_ADDRESS, boot_count + 1);

    // Seed our random generator with the boot count (I think this will overflow eventually)
    srand(boot_count);
    
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    angle = MIN;
    
    i = 0;
    duration = 100;
    
    while (1)
    {   
        if (TMR2_HasOverflowOccured()) {
            IO_RA1_Toggle();
            
            i++;
            if (i > duration) {
                i = 0;
                
                // Choose a random duration, stage1 has a long delay, otherwise quick delay
                if (stage1) {
                    duration = (rand() % 2000) + 1000;
                } else {
                    duration = (rand() % 200) + 50;
                }
                
                // Invert if we have a long or short delay next
                stage1 = !stage1;
                
                // Choose a random angle
                angle = (rand() % (MAX - MIN)) + MIN;
            }
            
            if (IO_RA1_LAT == 0) {
                // 0XF9 -> 20ms, but using slightly less since we have instructions in both cycles
                TMR2_SetPeriod(0xF0 - angle);
            } else {
                TMR2_SetPeriod(angle);
            }
        }
    }
}

/**
 End of File
*/