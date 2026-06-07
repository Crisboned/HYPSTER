/*
 * timer.h
 *
 *  Created on: 7 jun 2026
 *      Author: 23alv
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

extern volatile uint32_t tiempo_reaccion;

void TIMER_Start(void);
void TIMER_Stop(void);
void TIMER_Reset(void);


#endif /* INC_TIMER_H_ */
