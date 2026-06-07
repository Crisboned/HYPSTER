/*
 * timer.c
 *
 *  Created on: 7 jun 2026
 *      Author: 23alv
 */

#include "timer.h"

extern TIM_HandleTypeDef htim2;

volatile uint32_t tiempo_reaccion = 0;

void TIMER_Reset(void)
{
    tiempo_reaccion = 0;
}

void TIMER_Start(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

void TIMER_Stop(void)
{
    HAL_TIM_Base_Stop_IT(&htim2);
}
