#include "event_queue.h"



event_queue::event_queue()
{
}


event_queue::~event_queue()
{
}


bool event_queue::add_event(event_t evento)		//Agrego evento a cola de eventos
{
	bool validez = true;
	if (indice_add < SIZEEVENTQUEUE)		//Agrego solo si no me estoy saliendo del arreglo
	{
		event_qu[indice_add++] = evento;
	}
	else
	{
		validez = false;			//Si no es valido devuelvo un false
	}

	return validez;

}

void event_queue::clean_queue()
{
	for (int i = 0; i > SIZEEVENTQUEUE; i++)
	{
		event_qu[i] = END_OF_TABLE;
		indice_add = 0;
		indice_read = 0;
	}
}


event_t event_queue::read_queue()
{
	event_t respuesta = END_OF_TABLE;
	respuesta = event_qu[indice_read++];
	return respuesta;
}