#ifndef SYS_TICK_H
#define SYS_TICK_H

#include "glcd.h"

func_status_t sysTickConfig( void );
func_status_t delay_ms(uint32_t time);

#endif /* SYS_TICK_H */
