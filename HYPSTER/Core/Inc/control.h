/*
 * control.h
 *
 *  Created on: May 15, 2026
 *      Author: Critina Gomez
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_

#include <stdint.h>

#include "stm32f4xx_hal.h"

typedef enum {
    DIFICULTAD_FACIL = 0,
    DIFICULTAD_MEDIA,
    DIFICULTAD_DIFICIL
} Dificultad_t;

void Control_ActualizarDificultad(uint16_t adc_val);


void CONTROL_SetNumJugadores(uint8_t n);
uint8_t CONTROL_GetNumJugadores(void);

uint8_t CONTROL_ElegirJugadorAleatorio(void);
uint8_t CONTROL_EsPulsacionCorrecta(uint8_t jugador_led, int8_t jugador_pulsado);



#endif /* INC_CONTROL_H_ */
