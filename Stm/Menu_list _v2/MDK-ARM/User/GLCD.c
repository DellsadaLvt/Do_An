#include "GLCD.h"

#ifndef GLCD_GPIO
	#define GLCD_CLK				glcd_clk_Pin
	#define GLCD_DATA 			glcd_data_Pin
	#define GLCD_CLK_PORT 	glcd_clk_GPIO_Port
	#define GLCD_DATA_PORT 	glcd_data_GPIO_Port
#endif

#ifndef GLCD_FUNC_GPIO
	#define GLCD_CLK_SET(x)						HAL_GPIO_WritePin(GLCD_CLK_PORT, GLCD_CLK, ((uint8_t)(x)>0?GPIO_PIN_SET:GPIO_PIN_RESET))
	#define GLCD_DATA_SET(x)					HAL_GPIO_WritePin(GLCD_DATA_PORT, GLCD_DATA, ((uint8_t)(x)>0?GPIO_PIN_SET:GPIO_PIN_RESET))
	#define TRANSFER_BIT(data, bit) 	GLCD_DATA_SET((uint8_t)(data)&(uint8_t)(1u << (bit)))
#endif


#ifndef COMMAND_KEY
	#define COMMAND_KEY									((uint8_t)0xF8)
	#define DATA_KEY										((uint8_t)0xFA)
#endif

#ifndef BASIC_INSTRUCTION
	#define BASIC_INSTRUCTION 					((uint8_t)0x30) /* 8 bits interface, basic instruction */
#endif

#ifndef EXTERNAL_INSTRUCTION
	#define EXTERNAL_INSTRUCTION 				((uint8_t)0x34) /* 8 bits interface, external instruction */
	#define ENABLE_CGRAM_ADD 						((uint8_t)2U)
	#define GRAPHIC_MODE 								((uint8_t)0x36) /* 8 bits interface, external instruction, turn on graphic mode */
#endif


/*
	Mask functions
*/

static user_func_status_t user_delay( uint16_t time ){
	if( 0u == time )
		return fail;
	
	HAL_Delay(time);
	
	return oke;
}

#ifdef GLCD_USING_SPI
static user_func_status_t CS_write_pin(state_out_t state){
	if( state == set )
			HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
	else if( state == reset )
			HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
	else
		  return fail;
	
	return oke;
}

static user_func_status_t user_transmit(uint8_t data){
	HAL_SPI_Transmit(&hspi2, &data, 1u, 100u);
	
	return oke;
}

#else

static user_func_status_t user_transmit(const uint8_t data){
	GLCD_CLK_SET(0U);
	for(int8_t bit= 7; bit>= 0; bit-= 1){
		TRANSFER_BIT(data,bit);
		GLCD_CLK_SET(1U);
		GLCD_CLK_SET(0U);
	}
	
	return oke;
}

#endif

typedef enum {
	set,
	reset
} state_out_t;


/*
	---------------  Basic mode  ----------------
*/


static user_func_status_t glcd_command(const uint8_t data){
	user_transmit(COMMAND_KEY);
	user_transmit((uint8_t)(data&0xF0));
	user_transmit((uint8_t)(data<<4u));
	
	return oke;
}

static user_func_status_t glcd_data(const uint8_t data){
	user_transmit(DATA_KEY);
	user_transmit((uint8_t)(data&0xF0));
	user_transmit((uint8_t)(data<<4u));

	return oke;
}


static user_func_status_t glcd_user_CGRAM_init(void){
	glcd_command(EXTERNAL_INSTRUCTION);
	user_delay(1u);
	glcd_command(ENABLE_CGRAM_ADD);
	user_delay(1u);
	glcd_command(BASIC_INSTRUCTION);
	user_delay(1u);
	
	return oke;
}

static user_func_status_t set_index_GDRAM( const uint8_t vertical, const uint8_t horizontal){
	glcd_command(vertical);
	glcd_command(horizontal);
	
	return oke;
}

static user_func_status_t write_GDRAM( const uint8_t first_byte, const uint8_t second_byte){
	glcd_data(first_byte);
	glcd_data(second_byte);
	
	return oke;
}


/*
* Public function
*/
user_func_status_t glcd_display_clear(void){
	glcd_command(1u);
	user_delay(12u);
	
	return oke;
}

user_func_status_t glcd_init( void ){
	user_delay(50u);
	
	/* Function set */
	glcd_command(0x30);
	user_delay(1u);
	
	glcd_command(0x30);
	user_delay(1u);
	
	/* Display control */
	glcd_command(12u);
	user_delay(1u);
	
	glcd_display_clear();
	
	/* Entry mode */
	glcd_command(6u);
	user_delay(1u);
	
	/* Test display characters */
	glcd_command(LINE_2_BASE_DDRAM + 2u);
	glcd_data('A'); // 8*16
	
	return oke;
}

user_func_status_t glcd_test_CGRAM(void){
	glcd_user_CGRAM_init();
	
	for( uint8_t i= 0u; i< 16u; i+=1u){
		glcd_command(0x40 + i);   // 0x40 is the first line of the first address CGRAM
		glcd_data(0xfc);      // write into CGRAM data: higher byte
		glcd_data(0x0F);			// write into CGRAM data: lower byte
	}
	
	/* print data: Only 0000H, 0002H, 0004H and 0006H are acceptable */
	glcd_command(LINE_3_BASE_DDRAM + 6U);
	glcd_data(0);
	glcd_data(0);
	
	return oke;
}

user_func_status_t glcd_entry_graphic_mode( void ){
	/* function external set */
	glcd_command(GRAPHIC_MODE);
	
	return oke;
}


user_func_status_t glcd_clear_graphic(void){
		uint8_t vertical_add = 0x80;
		uint8_t horizontal_add = 0x80;
	/* Above part */
		for( uint8_t axis_y= 0; axis_y < 32u; axis_y+=1u){
			for( uint8_t axis_x= 0; axis_x < 8u; axis_x+=1u){
				set_index_GDRAM(VERTICAL_ADDRESS_BASE+ axis_y, HORIZONTAL_ADDRESS_BASE + axis_x);
				write_GDRAM(0u, 0u);
			}
		}
		
		/* Below part */
		for( uint8_t axis_y= 0u; axis_y < 32u; axis_y+=1u){
			for( uint8_t axis_x= 0; axis_x < 8u; axis_x+=1u){
				set_index_GDRAM(VERTICAL_ADDRESS_BASE+ axis_y, HORIZONTAL_ADDRESS_BASE + axis_x + 8u);
			write_GDRAM(0u, 0u);
			}
		}
	
	return oke;
}

user_func_status_t glcd_print_image(const unsigned char *const image){
	/* Above part: print arr components from 0 to 31  */
	uint8_t axis_x = 0u;
	uint8_t axis_y = 0u;
		for(; axis_y < 32u; axis_y+=1u){
			set_index_GDRAM(VERTICAL_ADDRESS_BASE+ axis_y, HORIZONTAL_ADDRESS_BASE);
			axis_x = 0u;
			for(; axis_x < 8u; axis_x+=1u){
				write_GDRAM(image[axis_y*16 + 2*axis_x], image[axis_y*16 + 2*axis_x+1u]);
			}
		}
		
		/* Below part: print arr components from 32 to 63*/
		for(; axis_y < 64u; axis_y+=1u){
			set_index_GDRAM(VERTICAL_ADDRESS_BASE+ axis_y - 32u, HORIZONTAL_ADDRESS_BASE+ 8u);
			axis_x = 0u;
			for(; axis_x < 8u; axis_x+=1u){
				write_GDRAM(image[axis_y*16 + 2*axis_x], image[axis_y*16 + 2*axis_x+1u]);
			}
		}
	
	return oke;
}











