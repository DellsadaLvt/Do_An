#include "systick.h"

volatile uint32_t u32g_TickCount = 0u;

func_status_t sysTickConfig( void ){
	/* disable systick */
	SysTick->CTRL = 0;
	/* init the RELOAD value */
	SysTick->LOAD = 72000u -1u;
	/* set priority  */
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
	/* reset current counter */
	SysTick->VAL = 0;
	/* select clock */
	SysTick->CTRL |= 0x04;
	/* enable systick interrupts */
	SysTick->CTRL |= 0x02;
	/* enable systick */
	SysTick->CTRL |= 0x01;

    return func_oke;
}

func_status_t delay_ms(uint32_t time){
	func_status_t ret= func_oke;
		uint32_t curent_count= u32g_TickCount;
		while(u32g_TickCount < (uint32_t)(curent_count + time));
	
	
	return ret;
}
