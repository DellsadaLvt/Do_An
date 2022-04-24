#include "pwm.h"

volatile uint16_t u16g_NCycles;

func_status_t pwm_GpioConfig(void){
	gpio_init(GPIOA, 0U, alt_output);
	
	return func_oke;
}

/* 
u16a_total_pulse = 10,000 ==> 1s
*/
func_status_t pwm_Timer2Chanel1Init(uint16_t u16a_total_pulse, uint8_t u8a_DutyCycle, uint16_t u16a_NCycles){
  /* enable clock TIM2 */
	rcc_enable(APB1, 0U);
	/* disable counter in CR1 register */
	TIM2->CR1 = 0u;
	/* reset counter */
	TIM2->CNT = 0u;
  /* 10,000 ticks -> 1s */	
	TIM2->PSC = 7200u -1u;                                                        
	/* set n ticks */
	TIM2->ARR = u16a_total_pulse - 1u; 
	/* chose mode pwm */
	TIM2->CCMR1 |= 0x60;                                                         /* choose 110 mode: pwm 1. 
																																									activate channel 1 */
	/* set width of pulse */
	TIM2->CCR1 = u16a_total_pulse*u8a_DutyCycle/100u;                            /* Duty cycle = 50% */
	/* config CCER register */
	TIM2->CCER |= 0x1111;		                                                     /* bit CC1E: 1, bit CC1P: 0, */
	/* set-up interupts */
	if( u16a_NCycles ){
		/* update ncycles value */
		u16g_NCycles = u16a_NCycles;
		/* clear interrupt flag */
		TIM2->SR &= ~(0x01);
		/* set update interrupts enable */
		TIM2->DIER |= 0x01;
		/* enable interrupts */
		NVIC->ISER[0] |= 0x01<<28;
	}																																						 /* capture/compare enable all */
	/* update generate */
	TIM2->EGR |= 0x01;
	/* enable counter in CR1 register */
	TIM2->CR1 |= 0x01;
	
	return func_oke;
}


