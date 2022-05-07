#include "Config.h"

typedef enum{
	APB1,
	APB2,
	AHP
} rcc_source_t;


func_status_t rcc_enable( rcc_source_t rcc_source, uint8_t peripheral);
func_status_t rcc_disable( rcc_source_t rcc_source, uint8_t peripheral);



