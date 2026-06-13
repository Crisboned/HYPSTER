/*
 * control.c
 *
 *  Created on: May 15, 2026
 *      Author: Critina Gomez
 */

#include "control.h"
#include <stdlib.h>

static uint8_t num_jugadores = 4; // por defecto

void CONTROL_SetNumJugadores(uint8_t n)
{
    if (n >= 1 && n <= 4)
        num_jugadores = n;
}

uint8_t CONTROL_GetNumJugadores(void)
{
    return num_jugadores;
}

uint8_t CONTROL_ElegirJugadorAleatorio(void)
{
    return rand() % num_jugadores;
}

uint8_t CONTROL_EsPulsacionCorrecta(uint8_t jugador_led, int8_t jugador_pulsado)
{
    return jugador_led == jugador_pulsado;
}
// Control Potenciómetro
// 12 bits (4095)
#define ADC_MAX     4095u
#define TH_FACIL    (ADC_MAX / 3u)
#define TH_MEDIA    (2u * ADC_MAX / 3u)


// LEDs de dificultad
#define LED_FACIL_GPIO_Port   GPIOD
#define LED_FACIL_Pin         GPIO_PIN_12

#define LED_MEDIA_GPIO_Port   GPIOD
#define LED_MEDIA_Pin         GPIO_PIN_13

#define LED_DIFICIL_GPIO_Port GPIOD
#define LED_DIFICIL_Pin       GPIO_PIN_14

static Dificultad_t dificultad_actual = DIFICULTAD_FACIL;

void Control_ActualizarDificultad(uint16_t adc_val)
{
    Dificultad_t nueva;

    if (adc_val <= TH_FACIL) {
        nueva = DIFICULTAD_FACIL;
    } else if (adc_val <= TH_MEDIA) {
        nueva = DIFICULTAD_MEDIA;
    } else {
        nueva = DIFICULTAD_DIFICIL;
    }

    // Evitar reconfigurar LEDs si no cambia
    if (nueva != dificultad_actual) {
        dificultad_actual = nueva;

        // Apagamos todos
        HAL_GPIO_WritePin(LED_FACIL_GPIO_Port,  LED_FACIL_Pin,  GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_MEDIA_GPIO_Port,  LED_MEDIA_Pin,  GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_DIFICIL_GPIO_Port,LED_DIFICIL_Pin,GPIO_PIN_RESET);

        // Encendemos solo el de la dificultad actual
        switch (dificultad_actual) {
        case DIFICULTAD_FACIL:
            HAL_GPIO_WritePin(LED_FACIL_GPIO_Port,  LED_FACIL_Pin,  GPIO_PIN_SET);
            break;
        case DIFICULTAD_MEDIA:
            HAL_GPIO_WritePin(LED_MEDIA_GPIO_Port,  LED_MEDIA_Pin,  GPIO_PIN_SET);
            break;
        case DIFICULTAD_DIFICIL:
            HAL_GPIO_WritePin(LED_DIFICIL_GPIO_Port,LED_DIFICIL_Pin,GPIO_PIN_SET);
            break;
        }
    }
}

Dificultad_t Control_GetDificultad(void)
{
    return dificultad_actual;
}
