/***********************************************************************************************
    [FILE NAME]:   ADC.h

     [AUTHOR]:     Mariam Taha

  [DATE CREATED]:  28/4/2019

  [DESCRIPTION]:   This file contains all prototypes and preprocessor macros for ADC

 **********************************************************************************************/

#ifndef ADC_driver
#define ADC_driver

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


/*
#define FREE_RUNNING
#define POLLING
*/


extern uint16 g_ADC_value;

void ADC_init(void);
uint16 ADC_readChannel(uint8 channel);
void ADC_setCallBack(void(*ptr)(void));

#endif
