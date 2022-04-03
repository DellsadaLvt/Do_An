#include "gpio.h"

int main(void){
	gpio_init(GPIOA, 0U, output);
	
	while(1u){
		gpio_set_pin(GPIOA, 0U, pin_high);
		gpio_set_pin(GPIOA, 0U, pin_low);
	}
}



