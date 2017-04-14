/*
 * nrf_userconfig.h
 *
 *  Created on: Apr 6, 2017
 *      Author: George
 */

#ifndef NRF_USERCONFIG_H_
#define NRF_USERCONFIG_H_


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
#define nrfIRQpin BIT3

	/* CSN SPI chip-select */
#define nrfCSNport 4
#define nrfCSNportout P4OUT
#define nrfCSNpin BIT4

	/* CE Chip-Enable (used to put RF transceiver on-air for RX or TX) */
#define nrfCEport 3
#define nrfCEportout P3OUT
#define nrfCEpin BIT0





#endif /* NRF_USERCONFIG_H_ */
