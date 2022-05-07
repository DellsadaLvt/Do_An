#include "uart.h"

extern volatile uint8_t u8g_RtcCount;
extern volatile uint32_t u32g_TickCount;
extern volatile uint16_t u16g_NCycles;
extern volatile uint8_t u8g_ReadUart1;

void SysTick_Handler( void ){
	/* check flag counter */
	if( (SysTick->CTRL>>16u & 0x01u)  ){
		u32g_TickCount += 1u;
		//SysTick->CTRL &= ~((uint32_t)1u << 16u);
	}
}

void RTC_IRQHandler(){
	if( (RTC->CRL & 0x01) && ( RTC->CRH & 0x01 ) ){
		u8g_RtcCount += 1u;
		
	if( u8g_RtcCount > 250u ){
		u8g_RtcCount = 0u;
	}
		
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

/*******************************************************
 *
 *
 ********************************************************/
void USART1_IRQHandler( void ){
	/* check error: parity, frame, noise, overun detect bit */
	if( (~(USART1->SR)&0x01u) && (~(USART1->SR>>1u)&0x01u) && (~(USART1->SR>>2u)&0x01u) && (~(USART1->SR>>3u)&0x01u) ){
		/* check interrupts flag and enable interrupts enable */
		if( ((USART1->SR>>5u)&0x01u) && ((USART1->CR1>>5u)&0x01u) ){
			u8g_ReadUart1 = USART1->DR;
			//uart_Uart1Trasmit( (char*)&u8g_ReadUart1 );
		}
	}
	/* reset all flag errors */
	USART1->DR;
}
