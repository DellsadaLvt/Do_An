#include "rcc.h"

typedef enum{
	pin_low,
	pin_high
} pin_state_t;

typedef enum{
	input,
	output
}pin_mode_t;

func_status_t gpio_init(GPIO_TypeDef* port, uint8_t pin_num, pin_mode_t pin_mode ); 
func_status_t gpio_set_pin(GPIO_TypeDef* port, uint8_t pin_num, pin_state_t pin_state ); 




