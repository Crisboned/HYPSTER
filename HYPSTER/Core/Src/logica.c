/*
 * logica.c
 *
 *  Created on: 15 jun. 2026
 *      Author: Critina Gomez
 */

#include "logica.h"
#include <stdlib.h>

uint8_t LOG_ElegirJugadorAleatorio(uint8_t num_jugadores)
{
    if(num_jugadores < 2 || num_jugadores > 4)
        return 0;

    return rand() % num_jugadores;
}

uint8_t LOG_EsPulsacionCorrecta(uint8_t jugador_led, int8_t jugador_pulsado)
{
    return (jugador_led == jugador_pulsado);
}

uint8_t LOG_RondaTerminada(uint8_t activaciones[], uint8_t num_jugadores)
{
    for(uint8_t i = 0; i < num_jugadores; i++)
    {
        if(activaciones[i] < 5)
            return 0;
    }
    return 1;
}

uint8_t LOG_CalcularGanador(uint32_t puntuacion[], uint8_t num_jugadores)
{
    uint8_t ganador = 0;
    uint32_t max = puntuacion[0];

    for(uint8_t i = 1; i < num_jugadores; i++)
    {
        if(puntuacion[i] < max)
        {
            max = puntuacion[i];
            ganador = i;
        }
    }

    return ganador;
}

void LOG_SumarPunto(uint32_t puntuacion[], uint8_t jugador)
{
    puntuacion[jugador]++;
}
