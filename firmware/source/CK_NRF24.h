
#ifndef CK_NRF24_H_
#define CK_NRF24_H_

#include "MKL27Z644.h"
#include "CK_TIME.h"
#include "CK_SPI.h"

void CK_NRF24_Init(void);
void CK_NRF24_SetRFModeIdle(void);
void CK_NRF24_SetRFModeRX(void);
void CK_NRF24_SetRFModeTX(void);
void CK_NRF24_SendMessage(uint8_t data[]);
void CK_NRF_SetChannelNumber(int num);

#endif /* CK_NRF24_H_ */
