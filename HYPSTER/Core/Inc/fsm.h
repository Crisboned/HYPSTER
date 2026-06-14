/*
 * fsm.h
 *
 *  Created on: 14 jun. 2026
 *      Author: Critina Gomez
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

typedef enum {
    ESTADO_INICIO,
    ESTADO_SELECT_JUGADORES,
    ESTADO_SELECT_DIFICULTAD,
    ESTADO_JUEGO,
    ESTADO_FIN
} Estado_t;

void FSM_Init(void);
void FSM_Update(void);

#endif /* INC_FSM_H_ */
