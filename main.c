/***************************************************************************************
//  George Nguyen

P3.1	SPI_CLK
P3.2	SPI_STE
P3.4	SPI_MOSI
P3.5	SPI_MISO





*/
#include <msp430.h>
#include "msp430x2xx.h" //register mapping
#include "msp430_nrf24l01p.h" //register mapping and nrf commands
#include "msp430x2xx_spi.h"
#include "msp430x2xx_uart.h"



struct DataPacket {

}


typedef struct DataPacket DataPacket;

int main(void) {

	LED_INIT();


	init_timer(); // Init 1000 Hz timer
	init_spO2_comms();
	init_spo2();
	SPI_init();
	init_control_unit_comms();
	ADC_PORT_INIT();
	DAC_PORT_INIT();
	MUX_PORT_INIT();



	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	P1DIR |= 0x01;					// Set P1.0 to output direction

	for(;;) {
		volatile unsigned int i;	// volatile to prevent optimization

		P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}
	
	SAMPLE_READY = 0;
	sei(); // Enable global interrupts

    while (1)
    {
    	while (SAMPLE_READY)
    	{
			cli();
    		SAMPLE_READY--;
			sei();
    		sampling_task();
    	}

		// Parse any received characters from control unit
		if (UCSR1A & (1<<RXC1))
		{
			uint8_t temp = UDR1;
			switch (temp)
			{
			case SPHINX2_HEADBOX_MODE_NORMAL:
				g_nSphinxMode = temp;
				writeDAC(0x0800);
				GND_DISCONNECT();
				break;
			case SPHINX2_HEADBOX_MODE_IMP:
				g_nSphinxMode = temp;
				g_nImpIndex = 0;
				g_nImpM1Cnt = 0;
				writeDAC(0x0800);
				GND_CONNECT();
				IMP_DISCONNECT_M1();
				g_bM1Imp = false;
				break;
			case SPHINX2_HEADBOX_MODE_RESET:
				g_nSphinxMode = temp;
				writeDAC(0x0800);
				GND_DISCONNECT();
				break;
			case SPHINX2_HEADBOX_MODE_CAL:
				g_nSphinxMode = temp;
				g_nCalIndex = 0;
				GND_DISCONNECT();
				break;
			case SPHINX2_HEADBOX_CMD_PRESSURE_CAL:
				calibrate_airflow();
				SAMPLE_READY=0;
				break;
			case SPHINX2_HEADBOX_CMD_XBI:
				SphinxSettings.bBipolarX = true;
				eeprom_update_block( &SphinxSettings, &SphinxSettingsEEPROM, sizeof(stSphinxEEPROM) );
				AUX_SWITCH_BIP();
				break;
			case SPHINX2_HEADBOX_CMD_XUNI:
				SphinxSettings.bBipolarX = false;
				eeprom_update_block( &SphinxSettings, &SphinxSettingsEEPROM, sizeof(stSphinxEEPROM) );
				AUX_SWITCH_UNI();
				break;
			case SPHINX2_O2_CONV:
				oximeter_configure(true);
				break;
			case SPHINX2_O2_AUTO:
				oximeter_configure(false);
				break;
			case SPHINX2_HEADBOX_SET_SERIAL:
				{
					char szTemp[16];
					char chRx = '\0';
					uint8_t nCnt =0;

					for (nCnt=0; nCnt<16;nCnt++)
					{
						szTemp[nCnt] = 0x00;
					}

					nCnt=0;

					bool bSuccess = false;
					SAMPLE_READY=0;
					while ((SAMPLE_READY<1000) && (bSuccess==false))
					{
						if (UCSR1A & (1<<RXC1))
						{
							SAMPLE_READY=0;
							chRx=UDR1;
							szTemp[nCnt]=chRx;
							if (chRx==0x00 || chRx==0x0A || chRx==0x0D)
							{
								bSuccess=true;
								szTemp[nCnt]=0x00;
							}
							nCnt++;
							if (nCnt>=16)
							{
								bSuccess = true;
							}
						}
					}

					if (bSuccess)
					{
						memcpy(SphinxSettings.szSerialNumber, szTemp, 16);
						eeprom_update_block( &SphinxSettings, &SphinxSettingsEEPROM, sizeof(stSphinxEEPROM) );
						send_eeprom_packet();
					}
					SAMPLE_READY=0;
				}
				break;
			case SPHINX2_HEADBOX_RESET:
				// Reset headbox
				cli();
				wdt_enable(WDTO_15MS);
				while(1);
				break;
			default:
				g_nSphinxMode = SPHINX2_HEADBOX_MODE_NORMAL;
				writeDAC(0x0800);
				GND_DISCONNECT();
				break;
			}
		}
    }

	return 0;
}


//Interrupt for SPO2
ISR( USART0_RX_vect ) //USART0 Rx complete
{
	oximeter_buffer[oximeter_bytes++] = UDR0;
}

ISR(TIMER1_COMPA_vect) //Timer/Counter1 Compare Match A
{
	SAMPLE_READY++;
}

ISR(TIMER1_OVF_vect) //timer/counter1 overflow
{

}

ISR(BADISR_vect)
{

}

