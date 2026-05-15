/*
 * leds.h
 *
 *  Created on: 15 may. 2026
 *      Author: Critina Gomez
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

void LEDS_Init(void);
void LEDS_AllOff(void);
void LEDS_On(uint8_t id);
uint8_t LEDS_Random(void);


#endif /* INC_LEDS_H_ */
