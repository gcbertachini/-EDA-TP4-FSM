/*
* File:   fsm.cpp
* Author: Grupo 1
*
* Created on March 24, 2018
*/

/*******************************************************************************
                     INCLUDED HEADERS 
******************************************************************************/
#include "fsm.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************************************************
						ENUMS, STRUCTS Y TYPEDEFS
******************************************************************************/
typedef enum {CLEAR,FETCH}mode_t;//typedef for the function get_ev


 /*******************************************************************************
					  CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PRINT 0        //variable for debugging
#define SIZEEVENTQUEUE 10000 //Size of event queue
#define PRINTF 0 //macro for debugging while using printf instead of PDCurse library
//Messages
#define MSJ_INTRO "Press the following keys to simulate an event:\nMOVE_RECEIVED:'b'\n MOVE_SENT: 'c'\nACK : 'd'\n" \
				  "TIME_OUT : 'e'\nTIME_OUT_2 : 'f\nQUIT : 'g'\nERROR : 'h'\nGARBAGE : 'i'\nRESET : 'j'\n I_AM_READY_SERVER : 'k'\n"\
	              "I_AM_READY_CLIENT : 'l' \nINVALID_ACKCODE : 'm'\nEND_COMMUNICATION : 'n'\n"



/*******************************************************************************
         FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
static edge_t* fsm(edge_t* ptr_actualstate , int event1);    //FSM Manager 

static event_t get_ev(mode_t mode);      //event handler

static void fun_exit(void);     

static void clean_event_queue(void);// 

static void do_nothing(void);//Dummy for the debugging of the protocol structure

static void dummy_printf(mode_t mode, event_t actual_event);

/*******************************************************************************
               STATIC VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
//Event queue for the FSM
static event_t event_queue[SIZEEVENTQUEUE];
//Control variable for the execution of the program
static switch_t running = ON;

//Pointer to the actual state
static edge_t *actual_state;

/*******************************************************************************
                             FSM STATES
******************************************************************************/

//Every state is represent by an array
static edge_t Initiating_state[];
static edge_t Waiting_for_ClientOK_state[];
static edge_t Finishing_configuration[];
static edge_t Looping_state[];
static edge_t Waiting_to_send_ACK_state[];
static edge_t Waiting_for_ACK_state[];
static edge_t Resending_MOVE[];
static edge_t Analyzing_ACK[];
static edge_t Sending_ERROR[];
static edge_t Switching_off[];


/*******************************************************************************
                           STATES INITIATION
******************************************************************************/

event_t * ptr_event = &event_queue[0]; //Initation of pointer


static edge_t Initiating_state[]=
{
	{I_AM_READY_SERVER, Waiting_for_ClientOK_state, do_nothing},
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Initiating_state, do_nothing }
};


static edge_t Waiting_for_ClientOK_state[]
{
	{ I_AM_READY_CLIENT, Finishing_configuration, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Waiting_for_ClientOK_state, do_nothing }
};

static edge_t Finishing_configuration[]=
{
	{ ACK, Looping_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};

static edge_t Looping_state[]=
{
	{ MOVE_RECEIVED, Waiting_to_send_ACK_state, do_nothing },
	{ MOVE_SENT, Waiting_for_ACK_state, do_nothing },
	{ QUIT, Waiting_for_ACK_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};

static edge_t Waiting_to_send_ACK_state[]=
{
	{ ACK, Waiting_for_ACK_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};

static edge_t Waiting_for_ACK_state[]
{
	{ TIME_OUT_2, Sending_ERROR, do_nothing },
	{ TIME_OUT, Resending_MOVE, do_nothing },
	{ ACK, Analyzing_ACK, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};

static edge_t Resending_MOVE[]=
{
	{ MOVE_SENT, Waiting_for_ACK_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};

static edge_t Analyzing_ACK[]=
{
	{ INVALID_ACKCODE, Switching_off, do_nothing },
	{ END_COMMUNICATION, Analyzing_ACK, fun_exit },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};

static edge_t Sending_ERROR[]=
{
	{ ERROR, Sending_ERROR, fun_exit },
	{ RESET, Initiating_state, do_nothing },
	{ GARBAGE ,Sending_ERROR, do_nothing },
	{ END_OF_TABLE,Finishing_configuration, do_nothing }
};




/******************************************************************************
*******************************************************************************
					GLOBAL FUNCTION DEFINITIONS
*******************************************************************************

******************************************************************************/
/**************************************************************
					MAIN_FSM
**************************************************************/
int  main(void)
{
	edge_t *actual_state = Initiating_state; //Creation of a pointer and initiation to the Initiating_state
	event_t actual_event;


#if PRINT
	int i;
	for (i = 0; i<SIZEEVENTQUEUE; i++)
	{
		printf("%d\t", event_queue[i]);
		event_queue[i] = 0;
	}
#endif
#if PRINTF 
	puts(MSJ_INTRO);
#endif
	
	while (running)  //check of the control variable to keep looping
	{
		while ((actual_event = get_ev(FETCH)) && (running)) //check if there is something in the queue or if the user decided to shut off the simulation
		{
#if PRINT
			printf("\nLa cola de eventos antes de ejecutar es:\n");
			for (i = 0; i<TAMCOLAEVENTO; i++)
				printf("%d\t", event_queue[i]);
#endif
#if PRINTF 
			dummy_printf(CLEAR, actual_event);
#endif

			actual_state = fsm(actual_state, actual_event); //The FSM returns the next state

#if PRINTF 
			dummy_printf(FETCH,0);
			puts("To exit press 'p'\n");
			fflush();
			if (getchar() == 'p')
				running = OFF;
#endif
			
		}

		
	}

	return EXIT_SUCCESS;
}


/**************************************************************
					FSM MANAGER
**************************************************************/
/*
*This function manage the FSM.
*
*INPUT:
*It receives the actual state of the fsm and the event that is hold in the edge 
*that contains the action routine.
*
*OUTPUT:
*The program returns a pointer to the next state.
*/
edge_t* fsm(edge_t* ptr2actual_state, int event1)
{

	while ((ptr2actual_state->event != event1) && (ptr2actual_state->event != END_OF_TABLE))
	{	//Search for the received event, it increase the pointer until the detection
		ptr2actual_state++;
	}

	//Runs the functions realted to that event
	(*ptr2actual_state->fun_trans)();

	return (ptr2actual_state->nextstate);
}


/**************************************************************
						GET_EV
**************************************************************/
/*
*Function that manage the extraction of events from the queue. 
*
*INPUT:
*Mode: If call in FETCH mode you obtain the next tracked event. In CLEAR mode, the queue is cleared.
*
*OUTPUT:
*The program returns the event extracted.
*/
static event_t get_ev(char mode)
{
	static int index = 0;        //index for event reading
	event_t extracted_event = END_OF_TABLE;

	if (mode) 
	{
		if (event_queue[index])   //if there is an event in the queue, it extracted the event and increment the index
		{
			extracted_event = event_queue[index++];    //if it can not found one, it retruns 0 that equals an END_OF_TABLE 
											  //if the latter happens the fsm does nothing and waits for the next event
		}
	}
	else //CLEAN mode
	{
		index = 0;
		clean_event_queue();
		ptr_event = &event_queue[0];
	}

	return extracted_event; //Devuelvo el evento extraido
}


/**************************************************************
					FUN_EXIT
**************************************************************/
void fun_exit(void)
{
	running = OFF;        //we turn off the flag that keeps the program looping
#if PRINTF
	puts("El programa finalizo con �xito.\n");
#endif

}

/**************************************************************
				   CLEAN_EVENT_QUEUE
**************************************************************/
static void clean_event_queue(void)
{
	for (uint i = 0; i < SIZEEVENTQUEUE; i++)
		event_queue[i] = END_OF_TABLE;
;
}

/**************************************************************
                    DUMMY_PRINTF
**************************************************************/
static void dummy_printf(mode_t mode, event_t actual_event)
{
	if (mode == FETCH)
	{	
		fflush(stdin);
		char letter = getchar();

		switch (letter)
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
			*ptr_event++ = ERROR;
			break;
		case('k'):
			*ptr_event++ = I_AM_READY_SERVER;
			break;
		case('l'):
			*ptr_event++ = I_AM_READY_CLIENT;
			break;
		case('m'):
			*ptr_event++ = INVALID_ACKCODE;
			break;
		case('n'):
			*ptr_event++ = END_COMMUNICATION;
			break;
		default:
			*ptr_event++ = GARBAGE;
			break;

		}
	}

	else
	{
		switch (actual_event)
		{
		case(MOVE_RECEIVED):
			puts("Se recibi� el evento MOVE RECEIVED");
			break;
		case(MOVE_SENT):
			puts("Se recibi� el evento MOVE SENT");
			break;
		case(ACK):
			puts("Se recibi� el evento ACK");
			break;
		case(TIME_OUT):
			puts("Se recibi� el evento TIME_OUT");
			break;
		case(TIME_OUT_2):
			puts("Se recibi� el evento TIME_OUT_2");
			break;
		case(QUIT):
			puts("Se recibi� el evento QUIT");
			break;
		case(ERROR):
			puts("Se recibi� el evento ERROR");
			break;
		case(I_AM_READY_SERVER):
			puts("Se recibi� el evento I_AM_READY_SERVER");
			break;
		case(I_AM_READY_CLIENT):
			puts("Se recibi� el evento I_AM_READY_CLIENT");
			break;
		case(INVALID_ACKCODE):
			puts("Se recibi� el evento INVALID_ACKCODE");
			break;
		case(END_COMMUNICATION):
			puts("Se recibi� el evento END_COMMUNICATION");
			break;
		default:
			puts("Se recibi� el evento GARBAGE");
			break;
		}
	}
}