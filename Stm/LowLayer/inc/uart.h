/**************************** 
* PA9:  Tx pin
* PA10: Rx pin
* select baud rate: 9600bps.
* caculate UART_DIV= 468.75
* div_faraction: 0.75*16= 12 => C
* div_mantissa: 468 -> 1D4
****************************/

#include "pwm.h"
#include "string.h"

#ifndef enaUart1
	#define enaUart1    ((uint32_t)0x4000)
#endif

#ifndef setUE
	#define setUE		 						((uint32_t)0x2000)
#endif

#ifndef setTE
	#define setTE    						((uint32_t)0x08)
#endif

#ifndef setTC
#define setTC    							((uint32_t)0x40)
#endif

#ifndef setRXNEIE
	#define setRXNEIE   				((uint32_t)0x20)
#endif

#ifndef setRE
	#define setRE      					((uint32_t)0x04)
#endif

#ifndef enaUSART_INT_NIVC 
	#define enaUSART_INT_NIVC   ((uint32_t)0x20)
#endif

#ifndef setPCE
	#define setPCE							((uint32_t)0x400)
#endif

#ifndef setPEIE
	#define setPEIE							((uint32_t)0x100)
#endif

#ifndef addUSART_DR
	#define addUSART_DR 				((uint32_t)0x40013804)
#endif

typedef enum{
	wl8Bits = 0,
	wl9Bits = 1
}wordLength;


typedef enum{
	oneSTB			= 0, // 1 Stop bit
	haftSTB   	= 1, //: 0.5 Stop bit
	twoSTB 			= 2, //: 2 Stop bits
	haftOneSTB 	= 3, // : 1.5 Stop bit
									//The 0.5 Stop bit and 1.5 Stop bit are not available for UART4 & UART5
}numberStopBit;


typedef enum{
	even 	= 0,
	old 
}parityOp;


typedef enum{
	Tx = 0x01,
	Rx,
	TRx
}receiveOrTransmit;


typedef enum{
	deActive = 0,
	active
}isActive;

/**/
#define enaPortA ((uint32_t)0x04)
#define enaAFIO  ((uint32_t)0x01)

typedef enum{
	outMaxSpeed10MHz 		= 0x01,
	outMaxSpeed2MHz  		= 0x02,
	outMaxSpeed50MHz  	= 0x03
}mode_option;

typedef enum{
	analog 			=  0x00,			//: Analog mode
	floatInput 	= 0x01,				//: Floating input (reset state)
	ppInput 		= 0x02				// Input with pull-up / pull-down
}CNF_input;

typedef enum{
	ppOut		= 0x00, 	//General purpose output push-pull
	odOut		= 0x01,   //: General purpose output Open-drain
	ppAfio	= 0x02, 	//: Alternate function output Push-pull
	opAfio	= 0x03   //: Alternate function output Open-drain
}CNF_output;


typedef struct{
	mode_option  mode;
	CNF_input    inp;
	CNF_output   onp;
}gpioInit;

/**********************/
/* Config GPIO */
func_status_t uart_Uart1GpioInit(void);

/**************** NORMAL USART USING INTERRUPTS ******************/
void uart_Uart1Trasmit( char *data );
void uart_Uart1Receive( void );

/**************** USART USING DMA ******************/
void USART_DMA_CONFIG_RX( void );
void USART_DMA_CONFIG_TX( void );
void DMA_RX(uint32_t *startAdd, uint32_t *endAdd, uint16_t u16NumberOfData);
void DMA_TX_INIT(uint32_t *startAdd, uint32_t *endAdd, uint16_t u16NumberOfData);	
void activeDMA_Channel4( isActive x , uint16_t u16reloadNumberOfData );
void activeDMA_Channel5( uint16_t u16NumberOfData );
void USART_DMA_CONFIG(  receiveOrTransmit mode  );
void DMA_MEMTOMEM ( uint32_t *startAdd, uint32_t *endAdd, uint16_t u16NumberOfData );
void DMA_MEMTOMEM_02 ( uint32_t *startAdd, uint32_t *endAdd, uint16_t u16NumberOfData );
void sample( void );
void sample2( void );


