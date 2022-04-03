#include "gpio.h"

func_status_t gpio_init( GPIO_TypeDef* port, uint8_t pin_num, pin_mode_t pin_mode ){
	func_status_t ret= func_oke;
	/* check parameters */
	if((15u < pin_num) || ((pin_mode != input) && (pin_mode != output)) ){
		ret= func_fail;
	}
	/* enable rcc */
	rcc_enable(APB2 ,(((uint32_t)port>>8u)&0xFF)/4u);
	
	/* set mode */
	if( pin_mode == input ){
	}
	else if( pin_mode == output ){
		if( pin_num > 7u){
			port->CRH &= ~(uint32_t)(0x0F << ((pin_num%8u)*4u));
			port->CRH |= (uint32_t)(2u << ((pin_num%8u)*4u));
		}
		else{
			port->CRL &= ~(uint32_t)(0x0F << (pin_num*4u));
			port->CRL |= (uint32_t)(2u << (pin_num*4u));
			
			/* Set pin in low level */
			port->BSRR |= 1u << (pin_num + 16u);
		}
	}
	
	return ret;
}
func_status_t gpio_set_pin(GPIO_TypeDef* port, uint8_t pin_num, pin_state_t pin_state ){
	func_status_t ret= func_oke;
	/* check parameters */
	if((15u < pin_num) || (pin_state != pin_high && (pin_state != pin_low)) ){
		ret= func_fail;
	}
	
	/* function */
	if( ret != func_fail){
		switch((uint8_t)pin_state){
			case pin_high:
				port->BSRR |= 1u << (pin_num);
				break;
			case pin_low:
				port->BSRR |= 1u << (pin_num + 16u);
				break;
			
			default:
				break;
		}
	}
	
	return ret;
}






