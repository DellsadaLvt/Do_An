#include "Config.h"

typedef enum{
	AHP,
	APB1,
	APB2
} rcc_source_t;


func_status_t rcc_enable( rcc_source_t rcc_source, uint8_t peripheral);




