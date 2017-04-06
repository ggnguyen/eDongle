/*
 * eDongle.h
 *
 *  Created on: Apr 3, 2017
 *      Author: George
 *  Here we define the configurations for the inputs, clock and sampling speed
 *  and any other configurable variables
 */

#ifndef EDONGLE_H_
#define EDONGLE_H_

#include "nrf24l01p.h"

/*eDongle nrf payload and Datapacket indices*/
#define ESCAPE			0
#define	PACKET_TYPE		1
#define	LEAD1_H			2
#define	LEAD1_L			3
#define	LEAD2_H			4
#define	LEAD2_L			5
#define	LEAD3_H			6
#define	LEAD3_L			7
#define	MLEAD_H			8
#define	MLEAD_L			9

#define ESCAPE_CHAR		0x18

/* LED */
#define EDONGLE_LED_RED
#define EDONGLE_LED_GREEN


/* nrf */
#define EDONGLE_NRF_CLK
#define EDONGLE_NRF_SPI
#define EDGONLE_NRF_SPEED	RF_DR_250KBPS
//#define EDGONLE_NRF_SPEED	RF_DR_1MBPS
//#define EDGONLE_NRF_SPEED	RF_DR_2MBPS

/* CPU clock cycles for the specified amounts of time--accurate minimum delays
 * required for reliable operation of the nRF24L01+'s state machine.
 */
/* Settings for 1MHz MCLK.
#define DELAY_CYCLES_5MS       5000
#define DELAY_CYCLES_130US     130
#define DELAY_CYCLES_15US      15
 */

/* Settings for 8MHz MCLK.
#define DELAY_CYCLES_5MS       40000
#define DELAY_CYCLES_130US     1040
#define DELAY_CYCLES_15US      120
 */

	/* Settings for 16MHz MCLK */
#define DELAY_CYCLES_5MS       80000
#define DELAY_CYCLES_130US     2080
#define DELAY_CYCLES_15US      240

/* Settings for 24MHz MCLK.
#define DELAY_CYCLES_5MS       120000
#define DELAY_CYCLES_130US     3120
#define DELAY_CYCLES_15US      360
 */

	/* SPI port--Select which USCI port we're using.
	 * Applies only to USCI devices.  USI users can keep these
	 * commented out.
	 */
//#define SPI_DRIVER_USCI_A 1
#define SPI_DRIVER_USCI_B 1


	/* Operational pins -- IRQ, CE, CSN (SPI chip-select)
	 */

	/* IRQ */
#define nrfIRQport 2
#define nrfIRQpin BIT2

	/* CSN SPI chip-select */
#define nrfCSNport 2
#define nrfCSNportout P2OUT
#define nrfCSNpin BIT1

	/* CE Chip-Enable (used to put RF transceiver on-air for RX or TX) */
#define nrfCEport 2
#define nrfCEportout P2OUT
#define nrfCEpin BIT0


/* ADC */
#define EDONGLE_ADC10_CLK
#define EDONGLE_ADC10_
#define EDONGLE_ADC10_
#define EDONGLE_ADC10_


/*SPI CONFIG*/

/*UART CONFIG */
#define EDONGLE_UART









#endif /* EDONGLE_H_ */
