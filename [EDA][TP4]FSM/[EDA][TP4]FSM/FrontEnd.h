#pragma once
#include"FSM_Server.h"
#include"curses.h"

#define MSJ_BIENVENIDA "BIENVENIDOS AL Siumlador \n SIMULA UN Servidor; USE EL TECLADO PARA ENVIAR EVENTOS; Y VEA EN LA PANTALLA LOS CAMBIOS DE ESTADOS\n "
#define LISTA_EVENTOS "la lista de eventos es \n B=MOVE_RECIEVED \n C = MOVE SENT \n D = ACK \n E = TIME_OUT \n F = TIME_OUT 2 \n G = QUIT \n H = ERROR \n J = RESET \n K= I_AM_READY \n M= INVALID_ACK_CODE \n N = END_COMMUNICATION \n O = VALID_ACK_CODE \n I= GARBAGE"
#define ESTADO "EL ESTADO ES = %s"
#define EVENT_ANT "EL EVENTO ANTERIOR ES = %s"
#define EVENT_ACT "EVENTO ACTUAL = %s"

#define ESPERANDOEVENTO "Esperando evento..."

#define PROCESANDOSTR "PROCESANDO... \n"

#define FILA 15
#define COLUMNA 30

#define Initiating_stateSTR "Initiating_state"

#define Waiting_for_ClientOK_stateSTR "Waiting_for_ClientOK_state");

#define Finishing_configurationSTR "Finishing_configuration"

#define Looping_stateSTR "Looping_state"

#define Waiting_to_send_ACK_stateSTR "Waiting_to_send_ACK_state"

#define Waiting_for_ACK_stateSTR "Waiting_for_ACK_state"

#define Resending_MOVESTR "Resending_MOVE"

#define Analyzing_ACKSTR "Analyzing_ACK"

#define Sending_ERRORSTR "Sending_ERROR"

typedef enum { ESPERANDO, PROCESANDO }modoFE_t;

void imprimir_simulacion(modoFE_t modo,event_t evento_actual, event_t evento_anterior, edge_t* estado, WINDOW*winTest);


