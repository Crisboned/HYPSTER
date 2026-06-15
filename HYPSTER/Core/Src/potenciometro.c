/*
 * potenciometro.c
 *
 *  Created on: 15 jun. 2026
 *      Author: Critina Gomez
 */
#include "potenciometro.h"
#include <stdlib.h>
#include "lcd.h"

//variables privadas

static ADC_HandleTypeDef *pot_adc = NULL;

static uint8_t num_jugadores = 4;

static Dificultad_t dificultad_actual = DIFICULTAD_FACIL;

//constantes ADC
#define ADC_MAX     4095u

#define TH_FACIL    (ADC_MAX / 3u)
#define TH_MEDIA    (2u * ADC_MAX / 3u)

void POT_Init(ADC_HandleTypeDef *hadc)
{
    pot_adc = hadc;
}
uint16_t POT_LeerADC(void)
{
    if(pot_adc == NULL)
        return 0;

    HAL_ADC_Start(pot_adc);

    HAL_ADC_PollForConversion(pot_adc,
                              HAL_MAX_DELAY);

    return HAL_ADC_GetValue(pot_adc);
}
void POT_SetNumJugadores(uint8_t n)
{
    if(n >= 2 && n <= 4)
    {
        num_jugadores = n;
    }
}

uint8_t POT_GetNumJugadores(void)
{
    return num_jugadores;
}

void POT_ActualizarDificultad(uint16_t adc_val)
{
    if(adc_val <= TH_FACIL)
    {
        dificultad_actual = DIFICULTAD_FACIL;
    }
    else if(adc_val <= TH_MEDIA)
    {
        dificultad_actual = DIFICULTAD_MEDIA;
    }
    else
    {
        dificultad_actual = DIFICULTAD_DIFICIL;
    }
}
Dificultad_t POT_GetDificultad(void)
{
    return dificultad_actual;
}
uint32_t POT_GetTiempoMax(void)
{
    switch(dificultad_actual)
    {
        case DIFICULTAD_FACIL:
            return 3000;

        case DIFICULTAD_MEDIA:
            return 2000;

        case DIFICULTAD_DIFICIL:
            return 1000;

        default:
            return 3000;
    }
}
void POT_SeleccionarJugadores(void)
{


    uint32_t inicio = HAL_GetTick();
    uint8_t jugadores_previos = 0;

    while((HAL_GetTick() - inicio) < 10000)
    {
        uint16_t adc_val = POT_LeerADC();

        uint8_t jugadores;
        if(adc_val <= 1365)
            jugadores = 2;
        else if(adc_val <= 2730)
            jugadores = 3;
        else
            jugadores = 4;
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

        	POT_SetNumJugadores(jugadores_previos);

}
void POT_SeleccionarDificultad(void)
{
    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("Selecciona");
    LCD_SetCursor(1,0);
    LCD_Print("dificultad");
    HAL_Delay(2000);

    uint32_t inicio = HAL_GetTick();
    Dificultad_t anterior = 255;

    while((HAL_GetTick() - inicio) < 10000)
    {
        uint16_t adc_val = POT_LeerADC();
        POT_ActualizarDificultad(adc_val);

        Dificultad_t actual = POT_GetDificultad();

        if(actual != anterior)
        {
            LCD_Clear();
            LCD_SetCursor(0,0);
            LCD_Print("Dificultad:");

            LCD_SetCursor(1,0);
            switch(actual)
            {
                case DIFICULTAD_FACIL: LCD_Print("FACIL"); break;
                case DIFICULTAD_MEDIA: LCD_Print("MEDIA"); break;
                case DIFICULTAD_DIFICIL: LCD_Print("DIFICIL"); break;
            }

            anterior = actual;
        }

        HAL_Delay(50);
    }
}


