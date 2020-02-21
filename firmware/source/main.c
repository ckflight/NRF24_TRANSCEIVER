
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL27Z644.h"

#include "CK_SPI.h"
#include "CK_TIME.h"
#include "CK_NRF24.h"

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    //Dataset:
	uint8_t dataset1[] = "11111111111111111111111111111111";
	uint8_t dataset2[] = "11111111111111111111111111111111";

    CK_NRF24_Init();//Default Channel 100
    CK_NRF_SetChannelNumber(2);

    while(1){
    	CK_NRF24_SendMessage(dataset1);
    	CK_delayMs(25);
    	CK_NRF24_SendMessage(dataset2);
		CK_delayMs(25);
    }

}
