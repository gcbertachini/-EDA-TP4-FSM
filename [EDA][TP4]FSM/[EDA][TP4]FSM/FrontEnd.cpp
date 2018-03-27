#include "FrontEnd.h"


#include <iostream>
#include <string>
#include <ctype.h>

using namespace std;

//extern event_t * ptr_event;



string decide_event(event_t evento);

string decide_action(event_t evento, edge_t* estado);

string decide_state(edge_t* estado);

void imprimir_simulacion(modoFE_t modo,event_t evento_actual, event_t evento_anterior, edge_t* estado,WINDOW*winTest,event_queue &ev_queue)
{
	
	int i = 0; // Variable donde recibo el input dle usuario
	int xWinTestPos, yWinTestPos;              //Variables en donde voy a guardar la posición del cursor.
	unsigned long long pressedKeyCounter = 0l; //Variable en donde voy a acumilar la cantidad de teclas presionadas antes de la 'Q'
	string STR_aux;	//String auxiliar para elejir strings mas facil
											  

		mvprintw(0,0,MSJ_BIENVENIDA);	//Siempre imprimo el mismo mensaje(quizas se puede sacar de esta funcion)
		if (modo == PROCESANDO)		
		{
			color_set(1, NULL);		// si estoy "procesando" el evento lo imprimo en otro color
			printw(PROCESANDOSTR);
			color_set(3, NULL);
		}
		else
		{
			printw("\n");
		}
		getyx(winTest, yWinTestPos, xWinTestPos);
		printw(LISTA_EVENTOS);		//Imprimo la lista de eventos posibles
		xWinTestPos += COLUMNA;	//Muevo el cursor para imprimir la proxima columna

		//decido el estado, luego lo imprimo
		STR_aux = decide_state(estado);
		mvprintw(yWinTestPos, xWinTestPos, ESTADO,STR_aux.c_str());

		yWinTestPos++;	//Me muevo a la proxima fila

		//en el modo esperando, se esta esperando al evento actual

		if (modo == ESPERANDO)
		{
			mvprintw(yWinTestPos, xWinTestPos, EVENT_ACT, ESPERANDOEVENTO);
		}
		// si no se esta esperando(se esta procesando) escribo el evento que llego
		else
		{		
			STR_aux = decide_event(evento_actual);
			mvprintw(yWinTestPos, xWinTestPos, EVENT_ACT, STR_aux.c_str());

		}
		yWinTestPos++;	//proxima fila
		//imprimo el evento anterior
		STR_aux = decide_event(evento_anterior);
		mvprintw(yWinTestPos, xWinTestPos, EVENT_ANT,STR_aux.c_str());

		yWinTestPos++;	//proxima fila
						
		STR_aux = decide_action(evento_actual, estado); //Imprimo la última acción ejecutada
		mvprintw(yWinTestPos, xWinTestPos, LAST_ACTION_EXCECUTED, STR_aux.c_str());


		//Backup de la posición actual del cursor antes de iterar...
		printw("\n");
		getyx(winTest, yWinTestPos, xWinTestPos);

		
		if (modo == ESPERANDO)
		{
			do
			{
				if ((i = getch()) != ERR)  //Si no hay input del usuario no 
					
				{
					i = tolower(i);
					switch (i)
					{
					case('b'):
						ev_queue.add_event(MOVE_RECEIVED);
						break;
					case('c'):
						ev_queue.add_event(MOVE_SENT);
						break;
					case('d'):
						ev_queue.add_event(ACK);
						break;
					case('e'):
						ev_queue.add_event(TIME_OUT);
						break;
					case('f'):
						ev_queue.add_event(TIME_OUT_2);
						break;
					case('g'):
						ev_queue.add_event(QUIT);
						break;
					case('h'):
						ev_queue.add_event(ERROR1);
						break;
					case('j'):
						ev_queue.add_event(RESET);
						break;
					case('k'):
						ev_queue.add_event(I_AM_READY);
						break;
					case('m'):
						ev_queue.add_event(INVALID_ACKCODE);
						break;
					case('n'):
						ev_queue.add_event(END_COMMUNICATION);
						break;
					case('o'):
						ev_queue.add_event(VALID_ACKCODE);
						break;
					case('i'):
						ev_queue.add_event(GARBAGE);
						break;

					default:
						if ((i > 'a') && (i < '}'))
							ev_queue.add_event(GARBAGE);
						break;

					}
				}
				
			} while (!((i>'a') && (i<'}')));

		}
}

string decide_event(event_t actual_event)
{
	string res;
	switch (actual_event)
	{
	case(MOVE_RECEIVED):
		res =  " MOVE_RECEIVED";
		break;
	case(MOVE_SENT):
		res = " MOVE_SENT";
		break;
	case(ACK):
		res = " ACK";
		break;
	case(TIME_OUT):
		res = " TIME_OUT";
		break;
	case(TIME_OUT_2):
		res = " TIME_OUT_2";
		break;
	case(QUIT):
		res = " QUIT";
		break;
	case(ERROR1):
		res = " ERROR";
		break;
	case(RESET):
		res = " RESET";
		break;
	case(I_AM_READY):
		res = " I_AM_READY";
		break;
	case(INVALID_ACKCODE):
		res = " INVALID_ACKCODE";
		break;
	case(VALID_ACKCODE):
		res = " VALID_ACKCODE";
		break;
	case(END_COMMUNICATION):
		res = " END_COMMUNICATION";
		break;
	case(END_OF_TABLE):
		res = " N/D";
		break;
	default:
		res = " GARBAGE";
		break;
	}
	return res;
}

string decide_action(event_t actual_event, edge_t* actual_state)
{
	string res;
	switch (actual_event)
	{
	case(MOVE_RECEIVED):
		res = " Se recibio un mensja MOVE";
		break;
	case(MOVE_SENT):
		if (actual_state == Waiting_for_ACK_state)
			res = " Se envio un mensaje MOVE";
		else
			res = " Se reenvio un mensaje MOVE ";
		break;
	case(ACK):
		res = " Se recibio un mensaje ACK";
		break;
	case(TIME_OUT):
		res = " El cliente no responde al envio de mensajes";
		break;
	case(TIME_OUT_2):
		res = " El cliente se encuentra no disponible, se procede a cerrar la comuniacion";
		break;
	case(QUIT):
		res = " Se solicita el cierre de la conexion";
		break;
	case(ERROR1):
		res = " Hubo un error en la conexion, se cierra la comunicacion";
		break;
	case(RESET):
		res = " Se procede a reinicar la conexion";
		break;
	case(I_AM_READY):
		if (actual_state == Waiting_for_ClientOK_state)
			res = " El Servidor se encuentra listo para establecer la conexion";
		else
			res = " El Cliente se encuentra listo para establecer la conexion";
		break;
	case(INVALID_ACKCODE):
		res = " Se detecto un ID invalido.  ";
		break;
	case(VALID_ACKCODE):
		res = " Se detectó un ID valido.";
		break;
	case(END_COMMUNICATION):
		res = " Se procede al cierre de la conexion";
		break;
	case(END_OF_TABLE):
		res = " N/D";
		break;
	default:
		res = " Se recibio informacion no compatible con el protocolo.";
		break;
	}
	return res;
}


string decide_state(edge_t* actual_state)
{
	string res;									
	if (actual_state == Initiating_state)
		res = Initiating_stateSTR;

	else if (actual_state == Waiting_for_ClientOK_state)
		res = Waiting_for_ClientOK_stateSTR;

	else if (actual_state == Finishing_configuration)
		res = Finishing_configurationSTR;

	else if (actual_state == Looping_state)
		res = Looping_stateSTR;

	else if (actual_state == Waiting_to_send_ACK_state)
		res = Waiting_to_send_ACK_stateSTR;

	else if (actual_state == Waiting_for_ACK_state)
		res = Waiting_for_ACK_stateSTR;

	else if (actual_state == Resending_MOVE)
		res = Resending_MOVESTR;

	else if (actual_state == Analyzing_ACK)
		res = Analyzing_ACKSTR;

	else if (actual_state == Sending_ERROR)
		res = Sending_ERRORSTR;
	return res;
}