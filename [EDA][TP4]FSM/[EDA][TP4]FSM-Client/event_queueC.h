#pragma once
#include "FSM_Client.h"

class event_queue
{
private:
	event_t event_qu[SIZEEVENTQUEUE];
	int indice_add = 0;

	int indice_read = 0;

public:
	event_queue();
	~event_queue();

	bool add_event(event_t evento);
	void clean_queue();

	event_t read_queue();


};

