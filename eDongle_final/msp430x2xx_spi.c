/* msp430_spi.c
 * Library for performing SPI I/O on a wide range of MSP430 chips.
 *
 * Serial interfaces supported:
 * 1. USI - developed on MSP430G2231
 * 2. USCI_A - developed on MSP430G2553
 * 3. USCI_B - developed on MSP430G2553
 * 4. USCI_A F5xxx - developed on MSP430F5172, added F5529
 * 5. USCI_B F5xxx - developed on MSP430F5172, added F5529
 *
 * Copyright (c) 2013, Eric Brundick <spirilis@linux.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright notice
 * and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT,
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <msp430.h>
#include "msp430x2xx_spi.h"
#include "nrf_userconfig.h"

/* USCI 16-bit transfer functions rely on the Little-Endian architecture and use
 * an internal uint8_t * pointer to manipulate the individual 8-bit segments of a
 * 16-bit integer.
 */

void spi_init()
{
	/* Configure ports on MSP430 device for USCI_B */
    P3SEL |= BIT0 | BIT1 | BIT2 | BIT3;
	//P3SEL2 &= ~(BIT1 | BIT2 | BIT3);

    ADC10AE0 &= ~(BIT5);  //P3.0 UCB0STE

//    P3SEL.0 = 1;     //P3.0 UCB0STE
//	ADC10AE0.5 = 0;  //P3.0 UCB0STE
//	P3SEL.1 = 1;     //P3.1 UCB0SIMO
//	P3SEL.2 = 1;    //P3.2 UCB0SOMI
//	P3SEL.3 = 1;    //P3.3 UCB0CLK

	/* USCI-B specific SPI setup */
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;  // SPI mode 0, master
	UCB0BR0 = 0x01;  // SPI clocked at same speed as SMCLK
	UCB0BR1 = 0x00;
	UCB0CTL1 = UCSSEL_2;  // Clock = SMCLK, clear UCSWRST and enables USCI_B module.
}

uint8_t spi_transfer(uint8_t inb)
{
	UCB0TXBUF = inb;
	while ( !(IFG2 & UCB0RXIFG) )  // Wait for RXIFG indicating remote byte received via SOMI
		;
	return UCB0RXBUF;
}

uint16_t spi_transfer16(uint16_t inw)
{
	uint16_t retw;
	uint8_t *retw8 = (uint8_t *)&retw, *inw8 = (uint8_t *)&inw;

	UCB0TXBUF = inw8[1];
	while ( !(IFG2 & UCB0RXIFG) )
		;
	retw8[1] = UCB0RXBUF;
	UCB0TXBUF = inw8[0];
	while ( !(IFG2 & UCB0RXIFG) )
		;
	retw8[0] = UCB0RXBUF;
	return retw;
}

uint16_t spi_transfer9(uint16_t inw)
{
	uint8_t p3dir_save, p3out_save, p3ren_save;
	uint16_t retw=0;

	/* Reconfigure I/O ports for bitbanging the MSB */
	p3ren_save = P3REN; p3out_save = P3OUT; p3dir_save = P3DIR;
	P3REN &= ~(BIT1 | BIT2 | BIT3);
	P3OUT &= ~(BIT1 | BIT2 | BIT3);
	P3DIR = (P3DIR & ~(BIT1 | BIT2 | BIT3)) | BIT1 | BIT3;
	P3SEL &= ~(BIT1 | BIT2 | BIT3);
	//P3SEL2 &= ~(BIT1 | BIT2 | BIT3);

	// Perform single-bit transfer
	if (inw & 0x0100)
		P3OUT |= BIT1;
	P3OUT |= BIT3;
	if (P3IN & BIT2)
		retw |= 0x0100;
	P3OUT &= ~BIT3;

	// Restore port states and continue with 8-bit SPI
	P3SEL |= BIT1 | BIT2 | BIT3;
	P3DIR = p3dir_save;
	P3OUT = p3out_save;
	P3REN = p3ren_save;

	retw |= spi_transfer( (uint8_t)(inw & 0x00FF) );
	return retw;
}
