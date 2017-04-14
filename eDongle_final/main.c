//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0
//
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  J. Stevenson
//  Texas Instruments, Inc
//  July 2011
//  Built with Code Composer Studio v5
//***************************************************************************************

#include <msp430.h>
#include "stdint.h"
#include "nrf_userconfig.h"
#include "nrf24l01p.h"
#include "msp430_nrf24l01p.h" //register mapping and nrf commands
#include "msp430x2xx_spi.h"
//#include "msp430x2xx_uart.h"
//#include "msp430x2xx_adc.h"
//#include "eDongle.h"

volatile unsigned int user;

struct DataPacket {

};


int main(void) {
//    LED_INIT();
//    init_timer(); // Init 1000 Hz timer
//    SPI_init();
//    ADC_PORT_INIT();
//    MUX_PORT_INIT();
//what to do here?

//  WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
//	P4DIR |= 0x01;					// Set P1.0 to output direction

	//--------------------------
   uint8_t addr[5];
   	uint8_t buf[32];

   	WDTCTL = WDTHOLD | WDTPW;
   	DCOCTL = CALDCO_16MHZ;
   	BCSCTL1 = CALBC1_16MHZ;
   	BCSCTL2 = DIVS_1;  // SMCLK = DCOCLK/2
   	// SPI (USCI) uses SMCLK, prefer SMCLK < 10MHz (SPI speed limit for nRF24 = 10MHz)

   	// Red, Green LED used for status
   	P4DIR |= 0x03;
   	P4OUT &= ~0x03;
   	//P4OUT = 0x01;

   	user = 0xFE;

   	/* Initial values for nRF24L01+ library config variables */
   	rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
   	rf_addr_width      = 5;
   	rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_0DBM;
   	rf_channel         = 120;

   	msprf24_init();  // All RX pipes closed by default
   	msprf24_set_pipe_packetsize(0, 32);
   	msprf24_open_pipe(0, 1);  // Open pipe#0 with Enhanced ShockBurst enabled for receiving Auto-ACKs
           // Note: Pipe#0 is hardcoded in the transceiver hardware as the designated "pipe" for a TX node to receive
           // auto-ACKs.  This does not have to match the pipe# used on the RX side.

   	// Transmit to 'rad01' (0x72 0x61 0x64 0x30 0x31)
   	msprf24_standby();
   	user = msprf24_current_state();
   	addr[0] = 0xDE;	addr[1] = 0xAD;	addr[2] = 0xBE;	addr[3] = 0xEF;	addr[4] = 0x00;
   	w_tx_addr(addr);
   	w_rx_addr(0, addr);  // Pipe 0 receives auto-ack's, autoacks are sent back to the TX addr so the PTX node
   			             // needs to listen to the TX addr on pipe#0 to receive them.

   	while(1){
   	//	__delay_cycles(800000);
   		if(buf[0]=='0'){buf[0] = '1';buf[1] = '0';}
   		else {buf[0] = '0';buf[1] = '1';}
   		w_tx_payload(32, buf);
   		msprf24_activate_tx();
   		//LPM4;

   		if (rf_irq & RF24_IRQ_FLAGGED) {
   			rf_irq &= ~RF24_IRQ_FLAGGED;

   			msprf24_get_irq_reason();
   			if (rf_irq & RF24_IRQ_TX){
   				P4OUT &= ~BIT1; // Red LED off
   				P4OUT |= 0x01;  // Green LED on
   			}
   			if (rf_irq & RF24_IRQ_TXFAILED){
   				P4OUT &= ~BIT0; // Green LED off
   				P4OUT |= BIT1;  // Red LED on
   			}

   			msprf24_irq_clear(rf_irq);
   			user = msprf24_get_last_retransmits();
   		}
   	}
  //--------------------------

//
//	for(;;) {
//		volatile unsigned int i;	// volatile to prevent optimization
//
//		P4OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR
//
//		i = 10000;					// SW Delay
//		do i--;
//		while(i != 0);
//	}

//    SAMPLE_READY = 0;
//    sei(); // Enable global interrupts
//
//    while (1)
//    {
//        while (SAMPLE_READY)
//        {
//            cli();
//            SAMPLE_READY--;
//            sei();
//            sampling_task();
//        }
//    }

	return 0;
}
//
//void initADC()
//{
//
//    P2DIR |=
//    P2SEL |= EDONGLE_LEAD_I | EDONGLE_LEAD_II | EDONGLE_LEAD_III
//
//
//    //P2
//    ADC10AE0.0 = 1;
//    ADC10AE0.1 = 1;
//    ADC10AE0.2 = 1;
//    ADC10AE0.3 = 1;
//    //P3.7, A7
//    ADC10AE0.7 = 1;
//
//}
//
//void initMUX()
//{
//    P1DIR |= EDONGLE_MUX | EDONGLE_SEL0 | EDONGLE_SEL1 | EDONGLE_SEL2;
//    P1SEL |= 0x00;
//}
//
//void initLED()
//{
//    P4DIR
//    P4SEL.0 = 0
//    P4SEL.1 = 0
//
//}
//
//void initSPI()
//{
//    P3DIR
//    P3SEL.0 = 1     //P3.0, UCB0STE
//    ADC10AE0.5 = 0  //P3.0, UCB0STE
//    P3SEL.1 = 1     //P3.1, UCB0SIMO
//    P3SEL.2 = 1     //P3.2, UCB0SOMI
//    P3SEL.3 = 1     //P3.3, UCB0CLK
//    P3SEL.4 = 1     //P3.4, UCA0TXD
//    P3SEL.5 = 1     //P3.5, UCA0RXD
//}
//
//void initUART()
//{
//
//}
////Interrupt for SPO2
//ISR( USART0_RX_vect ) //USART0 Rx complete
//{
//    oximeter_buffer[oximeter_bytes++] = UDR0;
//}
//
//ISR(TIMER1_COMPA_vect) //Timer/Counter1 Compare Match A
//{
//    SAMPLE_READY++;
//}
//
//ISR(TIMER1_OVF_vect) //timer/counter1 overflow
//{
//
//}
//
//ISR(BADISR_vect)
//{
//
//}
