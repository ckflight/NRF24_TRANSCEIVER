
#include "CK_NRF24.h"

#define RF_MODE_IDLE 0
#define RF_MODE_TX 1
#define RF_MODE_RX 2

int currentRFMode = 3;
int chipEnablePin = 0,slaveSelectPin = 4;
int channelNumber = 100;

void CK_NRF24_Init(void){

	CK_SPI_Init();

	//Set SS pin PTC4, PORTC Clock enabled in SPI_Config Section
	PORTC->PCR[slaveSelectPin] |= 1u<<8;
	GPIOC->PDDR |= 1u<<slaveSelectPin;
	GPIOC->PSOR |= 1u<<slaveSelectPin;

	CK_delayMs(50);

	//Set CE pin PE0
	SIM->SCGC5 |= 1u<<13;
	PORTE->PCR[chipEnablePin] |= 1u<<8;
	GPIOE->PDDR |= 1u<<chipEnablePin;
	GPIOE->PCOR |= 1u<<chipEnablePin;

	//Configure nRF24L01+ chip
	CK_SPI_WriteReg(0x07|0x20,0x40|0x20|0x10,GPIOC,slaveSelectPin);
	CK_SPI_WriteReg(0x1C|0x20,0x2F,GPIOC,slaveSelectPin);
	CK_SPI_WriteReg(0x1D|0x20,0x04|0x01,GPIOC,slaveSelectPin);
	CK_NRF24_SetRFModeIdle();

	GPIOC->PCOR |= 1u<<slaveSelectPin;
	CK_SPI_Transfer(0xE1);//Flush TX
	GPIOC->PSOR |= 1u<<slaveSelectPin;

	GPIOC->PCOR |= 1u<<slaveSelectPin;
	CK_SPI_Transfer(0xE2);//Flush RX
	GPIOC->PSOR |= 1u<<slaveSelectPin;

	CK_SPI_WriteReg(0x05|0x20,channelNumber & 0x7F,GPIOC,slaveSelectPin);//Set Channel
	CK_SPI_WriteReg(0x06|0x20,0x0E,GPIOC,slaveSelectPin);//0dBm, 2Mbps

}

void CK_NRF24_SetRFModeIdle(void){
	if(currentRFMode!=RF_MODE_IDLE){
		CK_SPI_WriteReg(0x00|0x20,0x08|0x04,GPIOC,slaveSelectPin);
		GPIOE->PCOR |= 1u<<chipEnablePin;
		currentRFMode = RF_MODE_IDLE;
	}
}
void CK_NRF24_SetRFModeRX(void){
	if(currentRFMode!=RF_MODE_RX){
		CK_SPI_WriteReg(0x00|0x20,0x08|0x04|0x02|0x01,GPIOC,slaveSelectPin);
		GPIOE->PSOR |= 1u<<chipEnablePin;
		currentRFMode = RF_MODE_RX;
	}
}
void CK_NRF24_SetRFModeTX(void){
	if(currentRFMode!=RF_MODE_TX){
		GPIOE->PCOR |= 1u<<chipEnablePin;
		CK_SPI_WriteReg(0x07|0x20,0x20|0x10,GPIOC,slaveSelectPin);
		CK_SPI_WriteReg(0x00|0x20,0x08|0x04|0x02,GPIOC,slaveSelectPin);
		GPIOE->PSOR |= 1u<<chipEnablePin;
		currentRFMode = RF_MODE_TX;
	}
}

void CK_NRF24_SendMessage(uint8_t data[]){
	//Find MessageSize
	int len = 0;
	while(data[len]!=0){//0 is null in ascii
		len++;
	}

	//Send Message
	if(len<=32){
		GPIOC->PCOR |= 1u<<slaveSelectPin;
		CK_SPI_Transfer(0xB0);//W_TX_PAYLOAD_NO_ACK
		uint8_t currentByte;
		for(int i=0;i<len;i++){
			currentByte = data[i];
			CK_SPI_Transfer(currentByte);
		}
		GPIOC->PSOR |= 1u<<slaveSelectPin;
		CK_NRF24_SetRFModeTX();
	}
}

void CK_NRF_SetChannelNumber(int num){
	channelNumber = num;
	CK_SPI_WriteReg(0x05|0x20,channelNumber & 0x7F,GPIOC,slaveSelectPin); // Set Channel
}


