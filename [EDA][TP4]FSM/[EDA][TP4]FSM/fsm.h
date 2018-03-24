/*
* File:   fsm.h
* Author: Grupo1
*
* Created on March 24, 2018
*/

#ifndef FSM_H
#define FSM_H

/*******************************************************************************
			CONSTANT AND MACRO DEFINITIONS USING #DEFINE
******************************************************************************/


/*******************************************************************************
						ENUMS, STRUCTS Y TYPEDEFS
******************************************************************************/


typedef enum { END_OF_TABLE, MOVE_RECEIVED, MOVE_SENT, ACK, TIME_OUT , 
			   TIME_OUT_2, QUIT, ERROR ,GARBAGE , RESET , I_AM_READY_SERVER,
			   I_AM_READY_CLIENT, INVALID_ACKCODE, END_COMMUNICATION } event_t;
/*
EVENT TABLE
MOVE_RECEIVED:'b'
MOVE_SENT: 'c'
ACK: 'd'
TIME_OUT: 'e'
TIME_OUT_2: 'f'
QUIT: 'g'
ERROR:'h' 
GARBAGE: 'i'
RESET: 'j'
I_AM_READY_SERVER: 'k'
I_AM_READY_CLIENT: 'l'
INVALID_ACKCODE: 'm'
END_COMMUNICATION: 'n'
*/

/* CONNECTION PROTOCOL EVENTS:
- MOVE_RECEIVED: The machine picks up a MOVE event from the other computer. We decide we have to make a difference between an incoming
MOVE and an outgoing one  beacuse the latter will have to wait for the ACK of their pair and in the case of the former 
the machine itself will send the ACK.
- MOVE_SENT: The machine sends a MOVE event to the other computer.
- ACK: The computer flags the success in the communication of a package.
- INVALID_ACK: The devide receives an ACK that doesn�t match with the ID of the MOVE sent.
- VALID_ACK: The device recieves an ACK that does match with the ID of the MOVE sent. It becomes available again for a new interaction.
- TIMEOUT: A previously defined time pass without receiving the ACK attach to a certain package already sent. The program proceeds
to resent the MOVE package.
- TIMEOUT_2: A previously defined time that flags that is occurring a problem in the conncection between the devices.
- QUIT: A device consciously want to end the connection between the pair, so it sends this particular message.
- GARBAGE: Some undetermined message is received, the connection is not working properly.
- RESET: The user wants to restart de protocol communication between the devices.
- I_AM_READY_SERVER: The server starts the protocol communication updating it�s status to ready.
- I_AM_READY_CLIENT: The client device acknowledges that it�s also ready to start the information exchange.
- END_COMMUNICATION: Both devices agreed to finish the connection between them.

*/

typedef enum { OFF, ON } switch_t;

typedef unsigned int uint;


/*****************************************************
*          DEFINITION OF EDGE
* **************************************************/
typedef struct edge edge_t;
struct edge
{
	event_t event;
	edge_t *nextstate;
	void(*fun_trans)(void);

};


#endif /* FSM_H */