
#include "CK_SPI.h"

uint32_t timeout;

void CK_SPI_Init(void){

	//Enable Clock to SPI0 and its pins
	SIM->SCGC5 |= 1u<<11;//PORTC
	SIM->SCGC4 |= 1u<<22;//SPI0

	//Configure SPI pins
	PORTC->PCR[5] = 1u<<9;//SCK
	PORTC->PCR[6] = 1u<<9;//MOSI
	PORTC->PCR[7] = 1u<<9;//MISO

	SPI0->BR = 0x00;//0x20=4Mhz,0x00=12MHz
	SPI0->C1 &= ~(1u<<2);//CPHA=0,CPOL=0 (MODE0 Of SAM3X)
	SPI0->C1 |= (1u<<6)|(1u<<4);//SPE,MSTR

}
uint8_t CK_SPI_Status(void){
	return SPI0->S;
}

uint8_t CK_SPI_WriteReg(uint8_t reg,uint8_t data, GPIO_Type* GPIOx_CS, int cs_pin){
	uint8_t val;

	GPIOx_CS->PCOR |= 1u<<cs_pin;

	CK_SPI_Transfer(reg);
	val = CK_SPI_Transfer(data);

	GPIOx_CS->PSOR |= 1u<<cs_pin;
	return val;

}

uint8_t CK_SPI_Transfer(uint8_t data){

	timeout = TIMEOUT;
	while((CK_SPI_Status() & 0x20) != 0x20){
		if(--timeout == 0x00){return 1;}
	}
	SPI0->DL = data;

	timeout = TIMEOUT;
	while((CK_SPI_Status() & 0x80) != 0x80){
		if(--timeout == 0x00){return 1;}
	}
	return SPI0->DL;

}



