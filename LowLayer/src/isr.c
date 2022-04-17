#include "stm32f10x.h"                  // Device header
#include "rcc.h"

extern volatile uint8_t u8g_RtcCount;
extern volatile uint16_t u16g_NCycles;

void RTC_IRQHandler(){
	if( (RTC->CRL & 0x01) && ( RTC->CRH & 0x01 ) ){
		u8g_RtcCount++;
		RTC->CRL &= ~0x01;
	}
}

void TIM2_IRQHandler( void ){
	static uint16_t u16a_Timer2Count= 1u;
	if( (TIM2->DIER & 0x01)  && (TIM2->SR& 0x01) ){
		TIM2->SR &= ~(0x01);
		u16a_Timer2Count += 1u;
		
		/* turn off timer 2 */
		if( u16a_Timer2Count >= u16g_NCycles ){
			/* disable counter in CR1 register */
			TIM2->CR1 = 0u;
			/* clear update interrupts enable */
		  TIM2->DIER &= ~(uint32_t)0x01;
			/* disable interrupts */
			NVIC->ISER[0] &= ~(uint32_t)(0x01<<28u);
			/* disable clock TIM2 */
			rcc_disable(APB1, 0u); 
		}
	}
}

