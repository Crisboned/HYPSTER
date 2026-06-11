/*
 * lcd.c
 *
 *  Created on: 7 jun 2026
 *      Author: 23alv
 */

#include "lcd.h"
#include <string.h>
#include <stdio.h>

/* Pines LCD */
#define LCD_RS_PORT GPIOC
#define LCD_RS_PIN  GPIO_PIN_0

#define LCD_E_PORT  GPIOC
#define LCD_E_PIN   GPIO_PIN_1

#define LCD_D4_PORT GPIOC
#define LCD_D4_PIN  GPIO_PIN_2

#define LCD_D5_PORT GPIOC
#define LCD_D5_PIN  GPIO_PIN_3

#define LCD_D6_PORT GPIOC
#define LCD_D6_PIN  GPIO_PIN_4

#define LCD_D7_PORT GPIOC
#define LCD_D7_PIN  GPIO_PIN_5

/* Funciones privadas */
static void LCD_SendNibble(uint8_t nibble);
static void LCD_SendCommand(uint8_t cmd);
static void LCD_SendData(uint8_t data);
static void LCD_PulseEnable(void);

/* -------------------------------------------------- */

static void LCD_PulseEnable(void)
{
    HAL_GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, GPIO_PIN_SET);
    HAL_Delay(5);

    HAL_GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, GPIO_PIN_RESET);
    HAL_Delay(5);
}

/* -------------------------------------------------- */

static void LCD_SendNibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(LCD_D4_PORT, LCD_D4_PIN,
                      (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D5_PORT, LCD_D5_PIN,
                      (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D6_PORT, LCD_D6_PIN,
                      (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D7_PORT, LCD_D7_PIN,
                      (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_PulseEnable();
}

/* -------------------------------------------------- */

static void LCD_SendCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);

    LCD_SendNibble(cmd >> 4);
    LCD_SendNibble(cmd & 0x0F);

    HAL_Delay(2);
}

/* -------------------------------------------------- */

static void LCD_SendData(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET);

    LCD_SendNibble(data >> 4);
    LCD_SendNibble(data & 0x0F);

    HAL_Delay(1);
}

/* -------------------------------------------------- */

void LCD_Init(void)
{
    HAL_Delay(50);

    //HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);

    LCD_SendNibble(0x03);
    HAL_Delay(5);

    LCD_SendNibble(0x03);
    HAL_Delay(5);

    LCD_SendNibble(0x03);
    //HAL_Delay(5);

    //LCD_SendNibble(0x02);
    //HAL_Delay(5);

    LCD_SendCommand(0x02);

    LCD_SendCommand(0x28); // 4 bits, 2 líneas
    LCD_SendCommand(0x0C); // display ON
    LCD_SendCommand(0x06); // incremento cursor
    LCD_SendCommand(0x01); // clear

    HAL_Delay(5);
}

/* -------------------------------------------------- */

void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
    HAL_Delay(2);
}

/* -------------------------------------------------- */

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    /*uint8_t address;

    if(row == 0)
        address = 0x80 + col;
    else
        address = 0xC0 + col;

    LCD_SendCommand(address);*/
	uint8_t address = (row == 0) ? 0x00 : 0x40;
	address += col;
	LCD_SendCommand(0x80 | address);
}

/* -------------------------------------------------- */

void LCD_Print(char *str)
{
    while(*str)
    {
        LCD_SendData(*str++);
    }
}

void LCD_PrintNumber(uint32_t num)
{
    char buffer[12];

    sprintf(buffer, "%lu", num);

    LCD_Print(buffer);
}
