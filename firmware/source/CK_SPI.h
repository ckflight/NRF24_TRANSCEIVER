
#ifndef CK_SPI_H_
#define CK_SPI_H_

#include <MKL27Z644.h>

#define TIMEOUT				300000

void CK_SPI_Init(void);
uint8_t CK_SPI_Status(void);
uint8_t CK_SPI_WriteReg(uint8_t reg,uint8_t data, GPIO_Type* GPIOx_CS, int cs_pin);
uint8_t CK_SPI_Transfer(uint8_t data);

#endif /* CK_SPI_H_ */
