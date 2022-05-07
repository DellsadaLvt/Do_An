#include "rcc.h"

func_status_t rcc_enable( rcc_source_t rcc_source, uint8_t peripheral){
	func_status_t ret= func_oke;
	/* Check parameters */
	if((2 < rcc_source) || (32 < peripheral)){
		ret= func_fail;
	}
	
	/* function */
	if(ret != func_fail){
		switch(rcc_source){
			case AHP:
				RCC->AHBENR |= (1u << peripheral);
				break;
			case APB1:
				RCC->APB1ENR |= (1u << peripheral);
				break;
			case APB2:
				RCC->APB2ENR |= (1u << peripheral);
				break;
			
			default:
				break;
		}
	}
	
	return ret;
}

func_status_t rcc_disable( rcc_source_t rcc_source, uint8_t peripheral){
	func_status_t ret= func_oke;
	/* Check parameters */
	if((1u < rcc_source) || (32u < peripheral)){
		ret= func_fail;
	}
	
	/* function */
	if(ret != func_fail){
		switch(rcc_source){
			case APB1:
				RCC->APB1ENR &= ~(uint32_t)(1u << peripheral);
				break;
			case APB2:
				RCC->APB2ENR &= !~(uint32_t)(1u << peripheral);
				break;
			
			default:
				break;
		}
	}
	
	return ret;
}









