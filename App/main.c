#include "pwm.h"

uint8_t counter= 250u;

int main(void){
	gpio_init(GPIOC, 13U, output);
	pwm_GpioConfig();
	configIntRTC();             /* module RTC is a timer use for delay function */
	pwm_Timer2Chanel1Init(20000u, 50u, 3u);  /* out 3pulses with duty cycle 50%, T= 2s -> 1s: high, 1s: low */
	delay(8u);   /* delay 8s */
	pwm_Timer2Chanel1Init(20000u, 80u, 3u);  /* out 3pulses with duty cycle 80%, T= 2s -> 1.6s: high, 0.4s: low */
	delay(8u);
	pwm_Timer2Chanel1Init(20000u, 20u, 0u);  /* out forever pulse ,with duty cycle 20%, T= 2s -> 0.4s: high, 1.6s: low  */

	while(1u){
		counter+=1u;
		delay(1u);
		if( counter % 2u )
			gpio_set_pin(GPIOC, 13U, pin_high);
		else
			gpio_set_pin(GPIOC, 13U, pin_low);
	}
}
