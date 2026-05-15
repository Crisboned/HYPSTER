/*
 * timer.h
 *
 *  Created on: 15 may. 2026
 *      Author: Critina Gomez
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f4xx_hal.h"

#define NUM_JUGADORES 4

// Contadores de tiempo por jugador (en ms)
extern volatile uint32_t timer_jugador[NUM_JUGADORES];

// Jugador actualmente midiendo
void TIMER_SetJugadorActivo(uint8_t jugador);

// Inicialización del temporizador
void TIMER_Init(void);

// Arrancar el temporizador
void TIMER_Start(void);

// Parar el temporizador
void TIMER_Stop(void);
//callback
void TIMER_Tick(void);

#endif /* INC_TIMER_H_ */
