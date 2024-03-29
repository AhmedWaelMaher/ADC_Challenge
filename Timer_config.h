/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer_config.h
 *
 * Description: Header file for the Timer link configurations AVR driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#ifndef TIMER_CNFG_H_
#define TIMER_CNFG_H_

#include "std_types.h"


typedef enum {T1,NUM_OF_TIMERS}Timers_Name;

#define TIMER_MODE NORMAL_MODE
#define NORMAL_MODE 0
#define CTC_MODE 1

typedef struct
{
	uint8 TIMER_ID;
	uint8 OC_ChA_Mode;
	uint8 OC_ChB_Mode;
	uint16 Normal_PreloadedValue;
	uint8 Prescaler;
	uint16 OCR_ChA;
	uint16 OCR_ChB;
	uint8 Interrupt_ChA;
	uint8 Interrupt_ChB;
}TIMER_SConfg;


extern TIMER_SConfg TIMER_cnfg_arr [NUM_OF_TIMERS];
extern TIMER_SConfg *TIMER_cnfg_ptr;



#endif /* TIMER_CNFG_H_ */
