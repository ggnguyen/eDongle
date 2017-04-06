/*
 * eDongle.c
 *
 *  Created on: Apr 3, 2017
 *      Author: George
 */





uint8_t MUX_OUTPUT[30] = {0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
						  0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
						  0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
						  0x40,0x44,0x45,0x46,0x47,0x00};

inline uint16_t ReadADC(void)
{
	uint16_t data;
	ADC_Select();
    SPI_TxRx(0x80); // Start conversion
    data = SPI_TxRx(0x00);
    data <<= 8;
    data |= SPI_TxRx(0x00);
    ADC_Deselect();
    return data;
}

inline void writeDAC( uint16_t data )
{
	SPSR |= 0x01;
	packed_word *word = (packed_word*)&data;
	DAC_Select();
    SPI_TxRx(word->by[1]);
    SPI_TxRx(word->by[0]);
    DAC_Deselect();
	SPSR &= 0xFE;
}
