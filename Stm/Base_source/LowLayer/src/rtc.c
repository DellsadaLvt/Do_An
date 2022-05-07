#include "rtc.h"

volatile uint8_t u8g_RtcCount= 0u;

static uint32_t setBit( uint8_t pos){
	return ((uint32_t)0x01 << pos);
}

func_status_t configIntRTC( void ){
	/*====== SECTION 1 ======*/
	/* set BKPEN */
	RCC->APB1ENR |= setBit(27u);
	/*  set PWREN */
	RCC->APB1ENR |= setBit(28u);
	/* set DBP */
	PWR->CR |= setBit(8u);
	
	/*=== SECTION 2 ===*/
	/* set LSEON */
	RCC->BDCR |= 0x01U;
	/* wait LSEON is stated */
	while( ((RCC->BDCR >> 1U) & (uint32_t)0x01)  != 1u);
	/* select RTC clock source */
	RCC->BDCR |= setBit(8u);
	RCC->BDCR &= ~setBit(9u);
	/* set RTCEN */
	RCC->BDCR |= setBit(15);
	
	/*=== SECTION 3 ===*/
	/* configuring RTC register */
	/* wait RTOFF */
	while(  ((RTC->CRL >> 5u)& (uint32_t)0x01)  != 0x01 );
	/* set CNF bit */
	RTC->CRL |= setBit( 4u );
	/* write one or more RTC register */
	/* RTC prescaler load register setup */
	RTC->PRLH = 0X0000;
	RTC->PRLL = 0x7fff;
	/* set SECIE */
	RTC->CRH |= 0x01;
	/* reset SECF */
	RTC->CRL &= ~0x01;
	/* clear CNF  */
	RTC->CRL &= ~setBit( 4u );
	/* wait  RTOFF*/
	while( ((RTC->CRL >> 5U) & 0x01) != 0x01 );
	
	/* enable interrupts in core  */
	NVIC->ISER[0] |= 0x08;
	
	return func_oke;
}

func_status_t delay(uint8_t time){
	func_status_t ret= func_oke;
	
	if(255 < time)
		ret= func_fail;
	
	if( u8g_RtcCount > 250u ){
		u8g_RtcCount = 0u;
	}
	
	if( ret != func_fail ){
		uint8_t curent_count= u8g_RtcCount;
		while(u8g_RtcCount < (uint8_t)(curent_count + time));
	}
	
	return func_oke;
}
