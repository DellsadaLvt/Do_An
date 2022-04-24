#include "glcd.h"

uint8_t counter= 250u;
uint8_t u8g_ReadUart1 = 0u;

int main(void){
	gpio_init(GPIOC, 13U, output);

	configIntRTC();             /* module RTC is a timer use for delay function */
	sysTickConfig(); 			/* Run as 1000Hz */
	
	// pwm_GpioConfig();
	// pwm_Timer2Chanel1Init(20000u, 50u, 3u);  /* out 3pulses with duty cycle 50%, T= 2s -> 1s: high, 1s: low */
	// delay(8u);   /* delay 8s */
	// pwm_Timer2Chanel1Init(20000u, 80u, 3u);  /* out 3pulses with duty cycle 80%, T= 2s -> 1.6s: high, 0.4s: low */
	// delay(8u);
	// pwm_Timer2Chanel1Init(20000u, 20u, 0u);  /* out forever pulse ,with duty cycle 20%, T= 2s -> 0.4s: high, 1.6s: low  */

	// uart_Uart1GpioInit();
	// uart_Uart1Trasmit("h");
	// uart_Uart1Receive();

	uint8_t msg[] = "Hello world";
	glcd_GpioInit();
	glcd_Init();
	glcd_display_clear();

	glcd_entry_basic_mode();
	glcd_basic_print_string(LINE_0_BASE_DDRAM, msg, strlen((char*)msg));
	delay(3u);
	glcd_display_clear();

	glcd_entry_graphic_mode();
	glcd_test_graphic_mode();

	

	while(1u){
		counter+=1u;
		delay(1u);
		if( counter % 2u )
			gpio_set_pin(GPIOC, 13U, pin_high);
		else
			gpio_set_pin(GPIOC, 13U, pin_low);

	}
}
