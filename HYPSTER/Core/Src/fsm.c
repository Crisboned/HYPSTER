/*
 * fsm.c
 *
 *  Created on: 15 jun. 2026
 *      Author: Critina Gomez
 */
#include "fsm.h"
#include "lcd.h"
#include "potenciometro.h"
#include "botones.h"
#include "leds.h"
#include "main.h"
#include "logica.h"
#include "timer.h"
#include <stdlib.h>
#include <string.h>


// Variables internas del módulo FSM (no globales del proyecto)
static volatile uint32_t contador = 0;
static volatile uint32_t espera = 0;
static volatile uint8_t ISR = 0;
extern volatile uint16_t adc_val;
extern volatile uint32_t tiempo_reaccion;
static volatile uint32_t tiempo_max = 3000;

static uint8_t activaciones[4] = {0};
static uint32_t puntuacion[4] = {0};
static uint8_t ganador = 0;
volatile uint8_t num_jugadores;

static Estado_t estado = ESTADO_INICIO;

static uint32_t tiempo_estado = 0;
static uint8_t primera_entrada = 1;
static uint8_t jugadores_mostrados = 0xFF;

static void Estado_Inicio(void);
static void Estado_SelectJugadores(void);
static void Estado_SelectDificultad(void);
static void Estado_Juego(void);
static void Estado_Fin(void);

void FSM_Init(void)
{
    estado = ESTADO_INICIO;
    for(int j=0; j<4; j++)
   	  {
   	      activaciones[j] = 0;
   	      puntuacion[j] = 0;
   	  }
}
void FSM_Update(void)
{
    switch(estado)
    {
        case ESTADO_INICIO:
            Estado_Inicio();
            break;

        case ESTADO_SELECT_JUGADORES:
            Estado_SelectJugadores();
            break;

        case ESTADO_SELECT_DIFICULTAD:
            Estado_SelectDificultad();
            break;

        case ESTADO_JUEGO:
            Estado_Juego();
            break;

        case ESTADO_FIN:
            Estado_Fin();
            break;
    }
}
static void Estado_Inicio(void)
{
    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("Pulsa boton");
    LCD_SetCursor(1,0);
    LCD_Print("para empezar");

    for(int i=0; i<4; i++)
    {
        activaciones[i] = 0;
        puntuacion[i] = 0;
    }

    contador = 0;
    ISR = 0;

    BOTONES_Clear();

    if (Boton_Menu_Pulsado()) {
        estado = ESTADO_SELECT_JUGADORES;
    }
}

static void Estado_SelectJugadores(void)
{
    if(primera_entrada)
    {
        LCD_Clear();

        LCD_SetCursor(0,0);
        LCD_Print("Jugadores:");

        tiempo_estado = HAL_GetTick();

        primera_entrada = 0;
    }

    uint16_t adc = POT_LeerADC();

    uint8_t jugadores;

    if(adc <= 1365)
        jugadores = 2;
    else if(adc <= 2730)
        jugadores = 3;
    else
        jugadores = 4;

    POT_SetNumJugadores(jugadores);

    if(jugadores != jugadores_mostrados)
    {
        LCD_SetCursor(1,0);
        LCD_Print("   ");

        LCD_SetCursor(1,0);
        LCD_PrintNumber(jugadores);

        jugadores_mostrados = jugadores;
    }

    if(Boton_Menu_Pulsado())
    {
    	Boton_Menu_Clear();
    	estado = ESTADO_SELECT_DIFICULTAD;
        primera_entrada = 1;
    }
}

static void Estado_SelectDificultad(void)
{
    if(primera_entrada)
    {
        LCD_Clear();

        LCD_SetCursor(0,0);
        LCD_Print("Dificultad:");

        primera_entrada = 0;
    }

    uint16_t adc = POT_LeerADC();

    POT_ActualizarDificultad(adc);

    LCD_SetCursor(1,0);

    switch(POT_GetDificultad())
    {
        case DIFICULTAD_FACIL:
            LCD_Print("FACIL   ");
            break;

        case DIFICULTAD_MEDIA:
            LCD_Print("MEDIA   ");
            break;

        case DIFICULTAD_DIFICIL:
            LCD_Print("DIFICIL ");
            break;
    }

    if(Boton_Menu_Pulsado())
    {
        tiempo_max = POT_GetTiempoMax();

        Boton_Menu_Clear();

        estado = ESTADO_JUEGO;
        primera_entrada = 1;
    }
}

static void Estado_Juego(void)
{
    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("Partida");
    LCD_SetCursor(1,0);
    LCD_Print("en curso...");
    HAL_Delay(1500);

    // Reiniciar variables del juego
    for(int i = 0; i < 4; i++)
    {
        activaciones[i] = 0;
        puntuacion[i] = 0;
    }

    // Bucle principal
    while(!LOG_RondaTerminada(activaciones, num_jugadores))
    {
    	if(Boton_Menu_Pulsado())
    	{
    	    TIMER_Stop();

    	    LEDS_AllOff();

    	    estado = ESTADO_INICIO;

    	    return;
    	}

    	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
        HAL_NVIC_DisableIRQ(EXTI1_IRQn);
        HAL_NVIC_DisableIRQ(EXTI2_IRQn);
        HAL_NVIC_DisableIRQ(EXTI3_IRQn);

        BOTONES_Clear();
        LEDS_AllOff();

        uint8_t led_actual;
        do {
            led_actual = LOG_ElegirJugadorAleatorio(num_jugadores);
        } while(activaciones[led_actual] >= 5);

        activaciones[led_actual]++;

        espera = 2000 + (rand() % 2001);
        HAL_Delay(espera);

        LEDS_On(led_actual);

        TIMER_Reset();
        TIMER_Start();

        HAL_NVIC_EnableIRQ(EXTI0_IRQn);
        HAL_NVIC_EnableIRQ(EXTI1_IRQn);
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);
        HAL_NVIC_EnableIRQ(EXTI3_IRQn);

        int8_t jugador = -1;

        while(jugador == -1)
        {
        	if(Boton_Menu_Pulsado())
        	{
        	    TIMER_Stop();

        	    LEDS_AllOff();

        	    estado = ESTADO_INICIO;

        	    return;
        	}

        	jugador = BOTONES_GetJugador();
            contador++;

            if(tiempo_reaccion >= tiempo_max)
            {
                TIMER_Stop();
                jugador = -2;
                break;
            }

            if(jugador == led_actual)
            {
                TIMER_Stop();
                break;
            }

            if(jugador != -1)
            {
                TIMER_Stop();
                break;
            }
        }

        if(jugador == led_actual)
        {
            puntuacion[jugador] += tiempo_reaccion;

            LEDS_AllOff();
            LEDS_On(led_actual);
            HAL_Delay(2000);
        }
        else if(jugador == -2)
        {
            puntuacion[led_actual] += tiempo_max;

            for(int k=0; k<10; k++)
            {
                LEDS_AllOff(); HAL_Delay(100);
                LEDS_On(led_actual); HAL_Delay(100);
            }
        }
        else
        {
            puntuacion[jugador] += tiempo_max;

            for(int k=0; k<5; k++)
            {
                LEDS_AllOff(); HAL_Delay(100);
                LEDS_On(led_actual); HAL_Delay(100);
            }
        }

        HAL_Delay(5000);
        contador = 0;
        ISR = 0;
    }

    ganador = LOG_CalcularGanador(puntuacion, num_jugadores);

    estado = ESTADO_FIN;
}

static void Estado_Fin(void)
{
    static uint8_t resultados_mostrados = 0;

    if(resultados_mostrados == 0)
    {
        // Mostrar puntuaciones
        for(uint8_t i = 0; i < num_jugadores; i++)
        {
            LCD_Clear();

            LCD_SetCursor(0,0);
            LCD_Print("Jugador ");
            LCD_PrintNumber(i + 1);

            LCD_SetCursor(1,0);
            LCD_Print("Pts:");
            LCD_PrintNumber(puntuacion[i]);

            HAL_Delay(3000);
        }

        // Mostrar ganador
        LCD_Clear();

        LCD_SetCursor(0,0);
        LCD_Print("GANADOR");

        LCD_SetCursor(1,0);
        LCD_Print("Jugador ");
        LCD_PrintNumber(ganador + 1);

        HAL_Delay(5000);

        resultados_mostrados = 1;
    }

    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("Pulsa para");

    LCD_SetCursor(1,0);
    LCD_Print("volver a inicio");

    if(Boton_Menu_Pulsado())
    {
        resultados_mostrados = 0;
        estado = ESTADO_INICIO;
    }
}

