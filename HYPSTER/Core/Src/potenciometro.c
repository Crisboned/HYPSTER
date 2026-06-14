/*
 * potenciometro.c
 *
 *  Created on: 14 jun. 2026
 *      Author: Critina Gomez
 */

#include "potenciometro.h"
#include "lcd.h"
#include "control.h"
#include "main.h"   // para hadc1, num_jugadores, tiempo_max, etc.
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern uint16_t adc_val;
extern uint8_t num_jugadores;
extern uint32_t tiempo_max;

// Arrays y variables que ya tienes en main
extern uint32_t activaciones[4];
extern uint32_t puntuacion[4];

void POT_SeleccionarJugadores(void)
{
    // Inicializa activaciones y puntuaciones como en tu main
    for(int j = 0; j < 4; j++)
    {
        activaciones[j] = 0;
        puntuacion[j] = 0;
    }

    // SELECCIÓN DE JUGADORES (copia literal de tu main)
    LCD_Clear();

    LCD_SetCursor(0,0);
    LCD_Print("Selecciona");

    LCD_SetCursor(1,0);
    LCD_Print("jugadores");

    HAL_Delay(2000);

    uint32_t inicio = HAL_GetTick();
    uint8_t jugadores_previos = 0;  // valor imposible

    while((HAL_GetTick() - inicio) < 10000)
    {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

        adc_val = HAL_ADC_GetValue(&hadc1);

        uint8_t jugadores;

        if(adc_val <= 1365)
        {
            jugadores = 2;
        }
        else if(adc_val <= 2730)
        {
            jugadores = 3;
        }
        else
        {
            jugadores = 4;
        }

        if(jugadores != jugadores_previos)
        {
            LCD_Clear();

            LCD_SetCursor(0,0);
            LCD_Print("Jugadores:");

            LCD_SetCursor(1,0);

            LCD_PrintNumber(jugadores);

            jugadores_previos = jugadores;
        }

        HAL_Delay(50);
    }

    CONTROL_SetNumJugadores(jugadores_previos);
    num_jugadores = CONTROL_GetNumJugadores();

    LCD_Clear();

    LCD_SetCursor(0,0);
    LCD_Print("JUGADORES:");

    LCD_SetCursor(1,0);
    LCD_PrintNumber(CONTROL_GetNumJugadores());

    HAL_Delay(3000);
}

void POT_SeleccionarDificultad(void)
{
    // SELECCIÓN DE DIFICULTAD (copia literal de tu main)
    LCD_Clear();

    LCD_SetCursor(0,0);
    LCD_Print("Selecciona");

    LCD_SetCursor(1,0);
    LCD_Print("dificultad");

    HAL_Delay(2000);

    uint32_t inicio = HAL_GetTick();
    Dificultad_t dificultad_anterior = 255;   // valor imposible

    while((HAL_GetTick() - inicio) < 10000)
    {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

        adc_val = HAL_ADC_GetValue(&hadc1);

        Control_ActualizarDificultad(adc_val);

        Dificultad_t dificultad_actual = Control_GetDificultad();

        if(dificultad_actual != dificultad_anterior)
        {
            LCD_Clear();

            LCD_SetCursor(0,0);
            LCD_Print("Seleccionando");

            LCD_SetCursor(1,0);

            switch(dificultad_actual)
            {
                case DIFICULTAD_FACIL:
                    LCD_Print("FACIL");
                    break;

                case DIFICULTAD_MEDIA:
                    LCD_Print("MEDIA");
                    break;

                case DIFICULTAD_DIFICIL:
                    LCD_Print("DIFICIL");
                    break;
            }

            dificultad_anterior = dificultad_actual;
        }

        HAL_Delay(50);
    }

    switch(Control_GetDificultad())
    {
        case DIFICULTAD_FACIL:
            tiempo_max = 3000;
            break;

        case DIFICULTAD_MEDIA:
            tiempo_max = 2000;
            break;

        case DIFICULTAD_DIFICIL:
            tiempo_max = 1000;
            break;
    }

    LCD_Clear();

    LCD_SetCursor(0,0);
    LCD_Print("DIFICULTAD:");

    switch(Control_GetDificultad())
    {
        case DIFICULTAD_FACIL:
            LCD_SetCursor(1,0);
            LCD_Print("FACIL");
            break;

        case DIFICULTAD_MEDIA:
            LCD_SetCursor(1,0);
            LCD_Print("MEDIA");
            break;

        case DIFICULTAD_DIFICIL:
            LCD_SetCursor(1,0);
            LCD_Print("DIFICIL");
            break;
    }

    HAL_Delay(3000);
}

