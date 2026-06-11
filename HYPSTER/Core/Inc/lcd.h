/*
 * lcd.h
 *
 *  Created on: 7 jun 2026
 *      Author: 23alv
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);
void LCD_PrintNumber(uint32_t num);

#endif /* INC_LCD_H_ */
