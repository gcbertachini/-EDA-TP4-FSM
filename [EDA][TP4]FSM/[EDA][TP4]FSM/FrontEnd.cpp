#include "FrontEnd.h"


#include <iostream>
#include <string>

using namespace std;

extern event_t * ptr_event;



string decide_event(event_t evento);

string decide_state(edge_t* estado);

void imprimir_simulacion(modoFE_t modo,event_t evento_actual, event_t evento_anterior, edge_t* estado,WINDOW*winTest)
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
		//Backup de la posición actual del cursor antes de iterar...
		printw("\n");
		getyx(winTest, yWinTestPos, xWinTestPos);

		
		if (modo == ESPERANDO)
		{
			do
			{
				i = getch();
				switch (i)
				{
				case('b'):
					*ptr_event++ = MOVE_RECEIVED;
					break;
				case('c'):
					*ptr_event++ = MOVE_SENT;
					break;
				case('d'):
					*ptr_event++ = ACK;
					break;
				case('e'):
					*ptr_event++ = TIME_OUT;
					break;
				case('f'):
					*ptr_event++ = TIME_OUT_2;
					break;
				case('g'):
					*ptr_event++ = QUIT;
					break;
				case('h'):
					*ptr_event++ = ERROR1;
					break;
				case('j'):
					*ptr_event++ = RESET;
					break;
				case('k'):
					*ptr_event++ = I_AM_READY;
					break;
				case('m'):
					*ptr_event++ = INVALID_ACKCODE;
					break;
				case('n'):
					*ptr_event++ = END_COMMUNICATION;
					break;
				case('o'):
					*ptr_event++ = VALID_ACKCODE;
					break;

				default:
					//*ptr_event++ = GARBAGE;
					break;

				}

			} while (((i != 'b')&& (i != 'c')&& (i != 'd')&&(i != 'e')&&(i != 'f')&&(i != 'g')&&(i != 'h')&&(i != 'm')&&(i != 'j')&&(i != 'k')&&(i != 'n')&&(i != 'o')&&(i != 'i')));

		}
}

string decide_event(event_t actual_event)
{
	string res;
	switch (actual_event)
	{
	case(MOVE_RECEIVED):
		res =  " MOVE RECEIVED";
		break;
	case(MOVE_SENT):
		res = "MOVE SENT";
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
		res = "END_OF_TABLE";
		break;
	default:
		res = " GARBAGE";
		break;
	}
	return res;
}

string decide_state(edge_t* actual_state)
{
	string res;									//NO falta desreferenciar esta estructura?
	if (actual_state == Initiating_state)
		res = "Initiating_state";

	else if (actual_state == Waiting_for_ClientOK_state)
		res = "Waiting_for_ClientOK_state";

	else if (actual_state == Finishing_configuration)
		res = "Finishing_configuration";

	else if (actual_state == Looping_state)
		res = "Looping_state";

	else if (actual_state == Waiting_to_send_ACK_state)
		res = "Waiting_to_send_ACK_state";

	else if (actual_state == Waiting_for_ACK_state)
		res = "Waiting_for_ACK_state";

	else if (actual_state == Resending_MOVE)
		res = "Resending_MOVE";

	else if (actual_state == Analyzing_ACK)
		res = "Analyzing_ACK";

	else if (actual_state == Sending_ERROR)
		res = "Sending_ERROR";
	return res;
}