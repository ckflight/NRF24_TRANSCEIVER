
#include "CK_TIME.h"

uint64_t tickCounter = 0;
int isFirst = 1;

void CK_delayMs(int msec){
	for(int i=0; i<3000*msec; i++);
}

uint64_t CK_milliSec(void){
	if(isFirst == 1){
		isFirst = 0;
		SysTick->LOAD = ((uint32_t)(47999));//1mS
		SysTick->VAL = 0;
		SysTick->CTRL |= (1u<<0)|(1u<<1)|(1u<<2);
	}
	return tickCounter;
}

uint64_t CK_microSec(void){
	if(isFirst == 1){
		isFirst = 0;
		SysTick->LOAD = ((uint32_t)(47999));//1mS
		SysTick->VAL = 0;
		SysTick->CTRL |= (1u<<0)|(1u<<1)|(1u<<2);
	}
	//If we count 1 micro at a time, slows down mcu so much. We count millisec and calculate micro
	return (tickCounter*1000) + ((SysTick->LOAD - SysTick->VAL) / (48));

}

void SysTick_Handler(void){
	tickCounter++;
}
