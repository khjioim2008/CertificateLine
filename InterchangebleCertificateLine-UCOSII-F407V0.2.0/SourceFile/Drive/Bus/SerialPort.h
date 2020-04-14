#ifndef __SerialPort_H
#define __SerialPort_H
#include "stm32f10x.h"
#include "ram.h"

typedef enum _tMANUAL_CONTROL_STATE
{
	MAN_SEND_MEMBRANE,
	MAN_STICK_MEMBRANE,
	MAN_TAKE_PAPER,
	MAN_INSERT_PAPER,
	MAN_FLOTSAM,
	MAN_ANALOG_DEBUG,
	MAN_FAULT_DEAL,
}MANUAL_CONTROL_STATE;
extern void ExternalControl(Motorns_Struct *Name, u8 m_action);


#endif


