#include "Menu_list.h"

#pragma pack(1)
typedef struct menu_list  {
	const struct menu_list *ptr_pre_menu;
	unsigned char title[20];
	
	unsigned char menu_list_0[20];
	const struct menu_list *ptr_menu_list_0;
	void (*ptr_func_list_0)(void);
	
	unsigned char menu_list_1[20];
	const struct menu_list *ptr_menu_list_1;
	
	unsigned char menu_list_2[20];
	const struct menu_list *ptr_menu_list_2;
	
	unsigned char menu_list_3[20];
	void *ptr_menu_list_3;
} menu_list_t;
#pragma pack()

const static menu_list_t main_menu;
const static menu_list_t sensor_menu;
const static menu_list_t actuator_menu;
const static menu_list_t setting_menu;
const static menu_list_t font_menu;

static void display_temp(void){
	static uint8_t status = 0u;
	status ^= 1u;
	uint8_t buff[2u];
	sprintf((char*)buff, "%d", status);
	
	glcd_graphic_print_characters(GDRAM_LINE(1u), BEGIN_LINE+6u, (const uint8_t*)buff, 1u);
}

/*-------- Main menu --------*/
const static menu_list_t main_menu = {
	NULL,
	"     Main Menu",
	"   Sensor", &sensor_menu, NULL,
	"   Actuator", &actuator_menu,
	"   Setting", &setting_menu,
	"   Line3", NULL
};

/*-------- Sensor menu ---------*/
const static menu_list_t sensor_menu = {
	&main_menu,
	"     Sensor Menu",
	"   Temperature", NULL, &display_temp,
	"   Humidity"   , NULL,
	"   Encoder"    , NULL,
	"   Line 3", NULL
};

/*-------- Actuator menu ---------*/
const static menu_list_t actuator_menu = {
	&main_menu,
	"     Actuator Menu",
	"   Motor"					, NULL, NULL,
	"   Led"   					, NULL,
	"   Cylinder"    		, NULL,
	"   Line 3", NULL
};

/*-------- Setting menu ---------*/
const static menu_list_t setting_menu = {
	&main_menu,
	"     Setting Menu",
	"   Font"					, &font_menu, NULL,
	"   Color"   			, NULL,
	"   Speed"    		, NULL,
	"   Line 3", NULL
};

const static menu_list_t font_menu = {
	&setting_menu,
	"     Font Menu",
	"   Font_Character"	, NULL, NULL,
	"   Font_digit"   	, NULL,
	"   "    						, NULL,
	"   Line 3", NULL
};

/*
*
* Mask functions
*
*/

static user_func_status_t user_delay( uint16_t u16_time_delay){
	if( 0u == u16_time_delay )
		return fail;
	
	HAL_Delay(u16_time_delay);
	
	return oke;
}

/*
*
* Local functions
*
*/
static user_func_status_t display_menu_list(const menu_list_t *const ptr_menu, const uint8_t u8_select ){
	if( NULL == ptr_menu)
			return oke;
	
	glcd_graphic_print_characters(GDRAM_LINE(0u), BEGIN_LINE, ptr_menu->title, strlen((char*)ptr_menu->title));
	glcd_graphic_print_characters(GDRAM_LINE(1u), BEGIN_LINE, ptr_menu->menu_list_0, strlen((char*)ptr_menu->menu_list_0));
	glcd_graphic_print_characters(GDRAM_LINE(2u), BEGIN_LINE, ptr_menu->menu_list_1, strlen((char*)ptr_menu->menu_list_1));
	glcd_graphic_print_characters(GDRAM_LINE(3u), BEGIN_LINE, ptr_menu->menu_list_2, strlen((char*)ptr_menu->menu_list_2));
	glcd_graphic_print_characters(GDRAM_LINE(0u), BEGIN_LINE + 8u, ptr_menu->menu_list_3, strlen((char*)ptr_menu->menu_list_3));
	glcd_graphic_print_characters(GDRAM_LINE(u8_select%4), BEGIN_LINE + 8u*(u8_select/4u), (const uint8_t*)">", 1u);


	return oke;
}


/*
*
* Public functions
*
*/
user_func_status_t display_main_list(const menu_handler_t *const menu){
	const static menu_list_t *ptr_menu = &main_menu;
	const menu_list_t* arr_subtitle[4u]= { ptr_menu->ptr_menu_list_0, ptr_menu->ptr_menu_list_1, ptr_menu->ptr_menu_list_2, ptr_menu->ptr_menu_list_3};
	
	switch(menu->event){
		case NEXT_EVENT:
			if( arr_subtitle[menu->selection - 1u] != NULL ){
				ptr_menu = arr_subtitle[menu->selection - 1u];
				glcd_clear_graphic();
			}
				break;
	
		case UNDO_EVENT:
			if( ptr_menu->ptr_pre_menu != NULL ){
				ptr_menu = ptr_menu->ptr_pre_menu;
				glcd_clear_graphic();
			}
			break;
			
		case CONFIRM_EVENT:
			if(NULL != ptr_menu->ptr_func_list_0){
				ptr_menu->ptr_func_list_0();
			}
			
		default:
			break;
	}
	
	display_menu_list(ptr_menu, menu->selection);	
	
	return oke;
}








