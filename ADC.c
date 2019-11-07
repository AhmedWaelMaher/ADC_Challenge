/***********************************************************************************************
    [FILE NAME]:   ADC.c

     [AUTHOR]:     Mariam Taha

  [DATE CREATED]:  28/4/2019

  [DESCRIPTION]:   This file contains all functions definitions of ADC driver

 **********************************************************************************************/

#include "ADC.h"

uint16 g_ADC_value = 0;

static void (*g_Temperature_function)(void) = NULL;

void ADC_init(void)
{
	ADMUX=0;
	ADMUX|=(1<<6);


#ifdef FREE_RUNNING

#ifdef POLLING
	ADCSRA=(1u << ADEN)|(1u << ADPS1)|(1u << ADPS0);
#else
	SREG |=(1u << 7u);
	ADCSRA=(1u << ADEN)|(1u << ADPS1)|(1u << ADPS0)|(1u << ADIE);
#endif

#else
	ADCSRA |=(1u << ADEN)|(1u << ADPS1)|(1u << ADPS0)|(1<<ADPS2)|(1u << ADATE)|(1<<ADIE);
	SFIOR |=(1u << ADTS2);
#endif
}


uint16 ADC_readChannel(uint8 channel)
{
#ifdef FREE_RUNNING
#ifdef POLLING
	ADMUX =(ADMUX&0xE0)|(channel&0x07);
	Set_bit(ADCSRA,ADSC);
	while(Bit_Is_Clear(ADCSRA,ADIF));
	Set_bit(ADCSRA,ADIF);
	g_ADC_value = ADC;
#else
	ADMUX =(ADMUX&0xE0)|(channel&0x07);
	Set_bit(ADCSRA,ADSC);
#endif
#else
	return g_ADC_value;
#endif
}

void ADC_setCallBack(void(*ptr)(void))
{
	g_Temperature_function = ptr;
}


ISR(ADC_vect)
{
	g_ADC_value = ADC;
	if(g_Temperature_function != NULL)
	{
		g_Temperature_function();
	}
}

