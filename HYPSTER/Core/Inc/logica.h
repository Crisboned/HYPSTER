/*
 * logica.h
 *
 *  Created on: 15 jun. 2026
 *      Author: Critina Gomez
 */

#ifndef INC_LOGICA_H_
#define INC_LOGICA_H_


#include <stdint.h>

// jugador aleatorio
uint8_t LOG_ElegirJugadorAleatorio(uint8_t num_jugadores);

// pulsacion correcta
uint8_t LOG_EsPulsacionCorrecta(uint8_t jugador_led, int8_t jugador_pulsado);

uint8_t LOG_RondaTerminada(uint8_t activaciones[], uint8_t num_jugadores);

uint8_t LOG_CalcularGanador(uint32_t puntuacion[], uint8_t num_jugadores);

#endif /* INC_LOGICA_H_ */
