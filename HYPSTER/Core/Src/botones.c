/*
 * botones.c
 *
 *  Created on: 15 may. 2026
 *      Author: Critina Gomez
 */

#include "botones.h"

static volatile int8_t jugador_pulsado = -1;
extern volatile uint8_t ISR;
volatile uint8_t boton_menu_flag = 0;

void BOTONES_Init(void)
{
    // Nada: CubeMX configura los EXTI
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    ISR = 1;
	if (GPIO_Pin == GPIO_PIN_0) jugador_pulsado = 0;		//J1
    else if (GPIO_Pin == GPIO_PIN_1) jugador_pulsado = 1;	//J2
    else if (GPIO_Pin == GPIO_PIN_2) jugador_pulsado = 2;	//J3
    else if (GPIO_Pin == GPIO_PIN_3) jugador_pulsado = 3;	//J4
	if (GPIO_Pin == BOTON_MENU_Pin) {
	        boton_menu_flag = 1;
	    }
}

int8_t BOTONES_GetJugador(void)
{
    return jugador_pulsado;
}

void BOTONES_Clear(void)
{
    jugador_pulsado = -1;
}

uint8_t Boton_Menu_Pulsado(void)
{
    if (boton_menu_flag) {
        boton_menu_flag = 0;
        return 1;
    }
    return 0;
}

void Boton_Menu_Clear(void)
{
    boton_menu_flag = 0;
}
