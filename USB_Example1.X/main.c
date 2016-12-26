/*
 * USB Test Program
 *
 * This file may be used by anyone for any purpose and may be used as a
 * starting point making your own application using M-Stack.
 *
 * It is worth noting that M-Stack itself is not under the same license as
 * this file.
 *
 * M-Stack is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  For details, see sections 7, 8, and 9
 * of the Apache License, version 2.0 which apply to this file.  If you have
 * purchased a commercial license for this software from Signal 11 Software,
 * your commerical license superceeds the information in this header.
 *
 * Alan Ott
 * Signal 11 Software
 * 2013-04-08
 */

#include "usb.h"
#include <xc.h>
#include <string.h>
#include "usb_config.h"
#include "usb_ch9.h"
#include "hardware.h"


#define _XTAL_FREQ  48000000

#define OUTPUT      0
#define INPUT       1

#define true        1
#define false       0

// LED defines for PICkit 2 Low Pin Count Demo Board
#define DS1_TRIS    TRISCbits.TRISC0
#define DS1_LAT     LATCbits.LATC0
#define DS1         PORTCbits.RC0

#define DS2_TRIS    TRISCbits.TRISC1
#define DS2_LAT     LATCbits.LATC1
#define DS2         PORTCbits.RC1

#define DS3_TRIS    TRISCbits.TRISC2
#define DS3_LAT     LATCbits.LATC2
#define DS3         PORTCbits.RC2

#define DS4_TRIS    TRISCbits.TRISC3
#define DS4_LAT     LATCbits.LATC3
#define DS4         PORTCbits.RC3



void oscillator_init() {
    OSCCONbits.IRCF = 0b1111; // 48 MHz internal clock

    // enable clock tuning from USB
    ACTCONbits.ACTSRC = 1;
    ACTCONbits.ACTEN = 1;
}

void timer_init() {
    //Timer2 Registers Prescaler= 4 - TMR2 PostScaler = 12 - PR2 = 250 - Freq = 1000.00 Hz - Period = 0.001000 seconds
    T2CON |= 88; // bits 6-3 Post scaler 1:1 thru 1:16
    T2CONbits.T2CKPS1 = 0; // bits 1-0  Prescaler Rate Select bits
    T2CONbits.T2CKPS0 = 1;
    PR2 = 250; // PR2 (Timer2 Match value)

    INTCONbits.GIE = 1; // global interrupts enabled
    INTCONbits.PEIE = 1; // peripheral interrupts enabled
    PIE1bits.TMR2IE = 1; // timer 2 interrupt enabled

    T2CONbits.TMR2ON = 1; // turn timer 2 on
}

void gpio_init() {
    // disable adc pins
    ANSELA = 0x00;
    ANSELC = 0x00;

    // LEDs as outputs
    DS1_TRIS = OUTPUT;
    DS2_TRIS = OUTPUT;  // RC0 and RC1 are tied to RA0 (D+) and RA1 (D-) on low pin count board
    DS3_TRIS = OUTPUT;
    DS4_TRIS = OUTPUT;

    DS1 = 0;
    DS2 = 0;
    DS3 = 1;
    DS4 = 0;
}

volatile uint32_t millis; // counter that increments every millisecond

int main(void)
{
	hardware_init();

    oscillator_init();
    gpio_init();
    timer_init();

	usb_init();

	unsigned char *buf = usb_get_in_buffer(1);
	memset(buf, 0xa0, EP_1_IN_LEN);

	while (1) {
        if ((millis % 1000) == 0) {
            __delay_ms(1);
            DS4_LAT ^= 1;
            DS3_LAT ^= 1;
        }
        
		if (usb_is_configured() && usb_out_endpoint_has_data(1)) {
			uint8_t len;
			const unsigned char *data;
			/* Data received from host */

			if (!usb_in_endpoint_halted(1)) {
				/* Wait for EP 1 IN to become free then send. This of
				 * course only works using interrupts. */
				while (usb_in_endpoint_busy(1))
					;

				len = usb_get_out_buffer(1, &data);
				memcpy(usb_get_in_buffer(1), data, EP_1_IN_LEN);
				usb_send_in_buffer(1, len);
			}
			usb_arm_out_endpoint(1);
		}

		#ifndef USB_USE_INTERRUPTS
		usb_service();
		#endif
	}

	return 0;
}

/* Callbacks. These function names are set in usb_config.h. */
void app_set_configuration_callback(uint8_t configuration)
{

}
uint16_t app_get_device_status_callback()
{
	return 0x0000;
}

void app_endpoint_halt_callback(uint8_t endpoint, bool halted)
{

}

int8_t app_set_interface_callback(uint8_t interface, uint8_t alt_setting)
{
	return 0;
}

int8_t app_get_interface_callback(uint8_t interface)
{
	return 0;
}

void app_out_transaction_callback(uint8_t endpoint)
{

}

void app_in_transaction_complete_callback(uint8_t endpoint)
{

}

static char buf[512];

static int8_t data_cb(bool transfer_ok, void *context)
{
	/* For OUT control transfers, data from the data stage of the request
	 * is in buf[]. */
	return 0;
}

int8_t app_unknown_setup_request_callback(const struct setup_packet *setup)
{
#define MIN(X,Y) ((X)<(Y)?(X):(Y))

	/* This handler handles request 254/dest=other/type=vendor only.*/
	if (setup->bRequest != 245 ||
	    setup->REQUEST.destination != 3 /*other*/ ||
	    setup->REQUEST.type != 2 /*vendor*/)
		return -1;

	if (setup->REQUEST.direction == 0/*OUT*/) {
		if (setup->wLength == 0) {
			/* There will be NO data stage. This sends back the
			 * STATUS stage packet. */
			usb_send_data_stage(NULL, 0, data_cb, NULL);
		}
		memset(buf,0,sizeof(buf));

		/* Set up an OUT data stage (we will receive data) */
		if (setup->wLength > sizeof(buf)) {
			/* wLength is too big. Return -1 to
			   refuse this transfer*/
			return -1;
		}
		usb_start_receive_ep0_data_stage(buf, setup->wLength, &data_cb, NULL);
	}
	else {
		/* Direction is 1 (IN) */
		size_t i;

		for (i = 0; i < sizeof(buf); i++) {
			buf[i] = sizeof(buf)-i;
		}

		/* Set up an OUT data stage (we will receive data) */
		if (setup->wLength > sizeof(buf)) {
			/* wLength is too big. Return -1 to
			   refuse this transfer*/
			return -1;
		}
		usb_send_data_stage(buf ,setup->wLength, data_cb, NULL);
	}

	return 0; /* 0 = can handle this request. */
#undef MIN
}

int16_t app_unknown_get_descriptor_callback(const struct setup_packet *pkt, const void **descriptor)
{
	return -1;
}

void app_start_of_frame_callback(void)
{

}

void app_usb_reset_callback(void)
{

}

#ifdef _PIC14E
void interrupt isr()
{
    if (PIR1bits.TMR2IF) { // timer 2 interrupt at 1000 Hz
        millis++;
        PIR1bits.TMR2IF = 0; // reset flag
        return;
    }

	usb_service();
}
#elif _PIC18

#ifdef __XC8
void interrupt high_priority isr()
{
	usb_service();
}
#elif _PICC18
#error need to make ISR
#endif

#endif