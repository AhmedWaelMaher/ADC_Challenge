/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer.c
 *
 * Description: Source file for the Timer AVR driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "Timer.h"

Timer_ID TimerID;

static uint8 Init_check[NUM_OF_TIMERS];

static volatile void (*g_TIMER0_OVF_callBackPtr)(void);
static volatile void (*g_TIMER0_CMP_callBackPtr)(void);
static volatile void (*g_TIMER1_OVF_callBackPtr)(void);
static volatile void (*g_TIMER1_CMPA_callBackPtr)(void);
static volatile void (*g_TIMER1_CMPB_callBackPtr)(void);
static volatile void (*g_TIMER2_OVF_callBackPtr)(void);
static volatile void (*g_TIMER2_CMP_callBackPtr)(void);

/*********************************************************
 *					Timer0 COMP Interrupt				 *
 *********************************************************/
void __vector_10 (void)
{
	if(g_TIMER0_CMP_callBackPtr != NULL)
	{
		g_TIMER0_CMP_callBackPtr();
	}
}

/*********************************************************
 *					Timer0 OVF Interrupt				 *
 *********************************************************/
void __vector_11 (void)
{
	if(g_TIMER0_OVF_callBackPtr != NULL)
	{
		g_TIMER0_OVF_callBackPtr();
	}
}

/*********************************************************
 *					Timer1 OVF Interrupt				 *
 *********************************************************/
void __vector_9 (void)
{
	if(g_TIMER1_OVF_callBackPtr != NULL)
	{
		g_TIMER1_OVF_callBackPtr();
	}

}

/*********************************************************
 *					Timer1 COMPB Interrupt				 *
 *********************************************************/
void __vector_8 (void)
{
	if(g_TIMER1_CMPB_callBackPtr != NULL)
	{
		g_TIMER1_CMPB_callBackPtr();
	}

}

/*********************************************************
 *					Timer1 COMPA Interrupt				 *
 *********************************************************/
void __vector_7 (void)
{
	if(g_TIMER1_CMPA_callBackPtr != NULL)
	{
		g_TIMER1_CMPA_callBackPtr();
	}

}



/*********************************************************
 *					Timer2 OVF Interrupt				 *
 *********************************************************/
void __vector_5 (void)
{
	if(g_TIMER2_OVF_callBackPtr != NULL)
	{
		g_TIMER2_OVF_callBackPtr();
	}
}

/*********************************************************
 *					Timer2 CMP Interrupt				 *
 *********************************************************/
void __vector_4 (void)
{
	if(g_TIMER2_CMP_callBackPtr != NULL)
	{
		g_TIMER2_CMP_callBackPtr();
	}
}



EnmTIMERError_t TIMER_init(uint8 TimerID)
{
	static uint8 retval=OK;
	Init_check[TimerID]=INITIALIZED;
	switch(TimerID)
	{
	case TIMER0:


		/*	uint8 TIMER_Mode  				done;
		 * 	uint8 OC_ChA_Mode				done;
					uint16 Normal_PreloadedValue  	done;
					uint8 Prescaler					done;
					uint8 Interrupt_ChA				done with Q;
					uint8 PWM_DUTYCYCLE;
					OCR chA							done*/


#if(TIMER_MODE==NORMAL_MODE)
		/****************************************************************
		 * 						  Timer0 Normal Mode					*
		 ****************************************************************/
		/*
		 * 1- FOC0 bit = 1 for non-PWM mode
		 * 2- WGM00 and WGM01 bits = 0 for Normal Mode
		 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin as it's not used in Normal mode
		 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
		 */
		TCCR0 = 0x80;

		/*
		 * Setting Timer Preloaded value
		 */
		if((TIMER_cnfg_arr[TimerID].Normal_PreloadedValue >= 0) && (TIMER_cnfg_arr[TimerID].Normal_PreloadedValue < 0xFF))
		{

			TCNT0 = TIMER_cnfg_arr[TimerID].Normal_PreloadedValue;
		}
		else if(TIMER_cnfg_arr[TimerID].Normal_PreloadedValue==NA)
		{

		}
		else
		{
			Init_check[TimerID]=NOT_INITIALIZED;
			retval = NOK;
		}



		/*
		 * Setting weather the timer operates in Interrupt or pooling mode
		 */
		if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == INTERRUPT)
		{
			TIMSK |= 0x01;
			SREG |= (1<<SREG_I);
		}
		else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == POOLING)
		{
			TIMSK &= ~0x01;
		}
		else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == NA)
		{

			TIMSK &= ~0x01;
		}
		else
		{
			Init_check[TimerID]=NOT_INITIALIZED;
			retval = NOK;
		}
		break;


#elif (TIMER_MODE==CTC_MODE)

		/****************************************************************
		 * 						  Timer0 CTC Mode						*
		 ****************************************************************/
		/*
		 * 1- FOC0 bit = 1 for non-PWM mode
		 * 2- WGM00 bit = 0 and WGM01 bit = 1 for CTC Mode
		 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
		 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
		 */
		TCCR0 = 0x88;
		/*
		 * Setting the Compare value
		 */
		if(TIMER_cnfg_arr[TimerID].OCR_ChA <= 0xFF)
		{
			OCR0 = TIMER_cnfg_arr[TimerID].OCR_ChA;
		}
		else
		{
			Init_check[TimerID]=NOT_INITIALIZED;
			retval = NOK;
		}

		/*
		 * Setting the OC pin Mode
		 */
		switch(TIMER_cnfg_arr[TimerID].OC_ChA_Mode)
		{
		case NA:
			break;
		case OC_DISABLED:
			break;

		case OC_TOGGLE:
			TCCR0 |= (1<<COM00);
			break;

		case OC_CLEAR:
			TCCR0 |= (1<<COM01);
			break;

		case OC_SET:
			TCCR0 |= (3<<COM00);
			break;

		default:
			Init_check[TimerID]=NOT_INITIALIZED;
			retval = NOK;
		}

		/*
		 * Setting weather the timer operates in Interrupt or pooling mode
		 */
		if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == INTERRUPT)
		{
			TIMSK |= 0x02;
			SREG |= (1<<SREG_I);
			DDRB |= 0x08;
		}
		else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == POOLING)
		{
			TIMSK &= ~0x02;
		}
		else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA==NA)
		{
			TIMSK &= ~0x02;
		}
		else
		{
			Init_check[TimerID]=NOT_INITIALIZED;
			retval = NOK;
		}



		break;

#endif


		/********************************************************************************
		 * 									Timer1										*
		 ********************************************************************************/
		case TIMER1:

			/*	uint8 TIMER_Mode  				;
			 * 	uint8 OC_ChA_Mode				;
			 * 	uint8 OC_ChB_Mode				;
									uint16 Normal_PreloadedValue  	;
									uint8 Prescaler					;
									uint8 Interrupt_ChA				;
									uint8 Interrupt_ChB 			;
									uint8 PWM_DUTYCYCLE             ;
									uint16 OCR chA	   				;
									uint16 OCR chB					;
									uint16 ICR						;
			 */
#if (TIMER_MODE==NORMAL_MODE)
			/****************************************************************
			 * 						  Timer1 Normal Mode					*
			 ****************************************************************/
			/*
			 * 1- FOC0 bit = 1 for non-PWM mode
			 * 2- WGM10, WGM11,WGM12 and WGM13 bits = 0 for Normal Mode
			 * 3- Check for prescaler and set CS12, CS11, CS10 values
			 */
			TCCR1A = 0b00000011;


			/*
			 * 4- Setting Timer preloaded value
			 */
			if((TIMER_cnfg_arr[TimerID].Normal_PreloadedValue >= 0)\
					&& (TIMER_cnfg_arr[TimerID].Normal_PreloadedValue < 0xffff ))
			{
				TCNT1L = TIMER_cnfg_arr[TimerID].Normal_PreloadedValue;
			}
			else if(TIMER_cnfg_arr[TimerID].Normal_PreloadedValue >= 0xffff )
			{
				TCNT1L = 0x00;
			}
			else
			{
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
			}
			/*
			 * 5- Setting weather the timer operates in Interrupt or pooling mode
			 */
			if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == INTERRUPT)
			{

				TIMSK |= 0x04;
				SREG |= (1u<<SREG_I);
				DDRD|=0x40;

			}
			else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == POOLING)
			{
				TIMSK &= ~0x04;
			}
			else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == NA)
			{
				TIMSK &= ~0x04;
			}
			else
			{
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
			}
			break;
#elif(TIMER_MODE==CTC_MODE)

			/****************************************************************
			 * 						  Timer1 CTC Mode						*
			 ****************************************************************/
			/*
			 * 1- FOC1A bit = 1 and FOC1B= 1 for non-PWM mode
			 * 2- WGM10 bit = 0 , WGM11= 0 ,WGM12= 1 and WGM13 bit = 0 for CTC Mode
			 * 3  Setting the OC pin Mode
			 */
			TCCR1A=0b00001100;

			/*
			 * Setting the OC pin Mode for Channel A
			 */
			switch(TIMER_cnfg_arr[TimerID].OC_ChA_Mode)
			{
			case OC_DISABLED:
				break;
			case NA:
				break;
			case OC_TOGGLE:
				TCCR1A |= (1u<<COM1A0);
				break;
			case OC_CLEAR:
				TCCR1A |= (1u<<COM1A1);
				break;
			case OC_SET:
				TCCR1A |= (3u<<COM1A0);
				break;
			default:
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
				break;
			}
			/*
			 * Setting the OC pin Mode for Channel B
			 */
			switch(TIMER_cnfg_arr[TimerID].OC_ChB_Mode)
			{
			case OC_DISABLED:
				break;
			case NA:
				break;
			case OC_TOGGLE:
				TCCR1A |= (1u<<COM1B0);
				break;
			case OC_CLEAR:
				TCCR1A |= (1u<<COM1B1);
				break;
			case OC_SET:
				TCCR1A |= (3u<<COM1B0);
				break;
			default:
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
				break;
			}

			/*
			 * Setting the Compare value of Timer1 Channel A
			 */
			if(TIMER_cnfg_arr[TimerID].OCR_ChA < 0xFFFF)
			{
				OCR1AL = TIMER_cnfg_arr[TimerID].OCR_ChA;
			}
			else if(TIMER_cnfg_arr[TimerID].OCR_ChA== NA_16)
			{

			}
			else
			{
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
			}

			/*
			 * Setting the Compare value of Timer1 Channel A
			 */
			if(TIMER_cnfg_arr[TimerID].OCR_ChB < 0xFFFF)
			{
				OCR1BL = TIMER_cnfg_arr[TimerID].OCR_ChB;
			}
			else if(TIMER_cnfg_arr[TimerID].OCR_ChB== NA_16)
			{

			}
			else
			{
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
			}

			/*
			 * Setting weather the timer1 CHA operates in Interrupt or pooling mode
			 */
			if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == INTERRUPT)
			{
				TIMSK |= 0x10;
				SREG |= (1u<<SREG_I);
				DDRD |= 0x40;

			}
			else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == POOLING)
			{
				TIMSK &= ~0x10;
			}
			else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == NA)
			{
				TIMSK &= ~0x10;
			}
			else
			{
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
			}
			/*
			 * Setting weather the timer1 CHB operates in Interrupt or pooling mode
			 */
			if(TIMER_cnfg_arr[TimerID].Interrupt_ChB == INTERRUPT)
			{
				TIMSK |= 0x08;
				SREG |= (1u<<SREG_I);
				DDRD |=0x20;
			}
			else if(TIMER_cnfg_arr[TimerID].Interrupt_ChB == POOLING)
			{
				TIMSK &= ~0x08;
			}
			else if(TIMER_cnfg_arr[TimerID].Interrupt_ChB == NA)
			{
				TIMSK &= ~0x08;
			}
			else
			{
				Init_check[TimerID]=NOT_INITIALIZED;
				retval = NOK;
			}

			break;


#endif
			/********************************************************************************
			 * 									Timer2										*
			 ********************************************************************************/
			case TIMER2:

#if(TIMER_MODE==NORMAL_MODE)
				/****************************************************************
				 * 						  Timer2 Normal Mode					*
				 ****************************************************************/

				/*
				 * 1- FOC0 bit = 1 for non-PWM mode
				 * 2- WGM00 and WGM01 bits = 0 for Normal Mode
				 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin as it's not used in Normal mode
				 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
				 */
				ASSR &= ~0x0F;
				TCCR2 = 0x80;

				/*
				 * Setting Timer Preloaded value
				 */
				if((TIMER_cnfg_arr[TimerID].Normal_PreloadedValue >= 0)\
						&& (TIMER_cnfg_arr[TimerID].Normal_PreloadedValue < 0xFF))
				{
					TCNT2 = TIMER_cnfg_arr[TimerID].Normal_PreloadedValue;
				}
				else if(TIMER_cnfg_arr[TimerID].Normal_PreloadedValue>=0xff)
				{

				}
				else
				{
					Init_check[TimerID]=NOT_INITIALIZED;
					retval = NOK;
				}



				/*
				 * Setting weather the timer operates in Interrupt or pooling mode
				 */
				if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == INTERRUPT)
				{
					TIMSK |= 0x40;
					SREG |= (1<<SREG_I);
					DDRD |= 0x80;
				}
				else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == POOLING)
				{
					TIMSK &= ~0x40;
				}
				else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == NA)
				{
					TIMSK &= ~0x40;
				}
				else
				{
					Init_check[TimerID]=NOT_INITIALIZED;
					retval = NOK;
				}
				break;



				/****************************************************************
				 * 						  Timer2 CTC Mode						*
				 ****************************************************************/

#elif(TIMER_MODE==CTC_MODE)



				/*
				 * 1- FOC0 bit = 1 for non-PWM mode
				 * 2- WGM00 bit = 0 and WGM01 bit = 1 for CTC Mode
				 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
				 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
				 */
				TCCR2 = 0x88;

				/*
				 * Setting the Compare value
				 */
				if(TIMER_cnfg_arr[TimerID].OCR_ChA < 0xFF)
				{
					OCR2 = TIMER_cnfg_arr[TimerID].OCR_ChA;
				}
				else if(TIMER_cnfg_arr[TimerID].OCR_ChA == 0xFF)
				{

				}
				else
				{
					Init_check[TimerID]=NOT_INITIALIZED;
					retval = NOK;
				}

				/*
				 * Setting the OC pin Mode
				 */
				switch(TIMER_cnfg_arr[TimerID].OC_ChA_Mode)
				{
				case OC_DISABLED:
					break;
				case NA:
					break;
				case OC_TOGGLE:
					TCCR2 |= (1<<COM20);
					break;

				case OC_CLEAR:
					TCCR2 |= (1<<COM21);
					break;

				case OC_SET:
					TCCR2 |= (3<<COM20);
					break;

				default:
					Init_check[TimerID]=NOT_INITIALIZED;
					retval = NOK;
				}

				/*
				 * Setting weather the timer operates in Interrupt or pooling mode
				 */
				if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == INTERRUPT)
				{
					TIMSK |= 0x80;
					SREG |= (1<<SREG_I);
					DDRD |= 0x80;

				}
				else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == POOLING)
				{
					TIMSK &= ~0x80;
				}
				else if(TIMER_cnfg_arr[TimerID].Interrupt_ChA == NA)
				{
					TIMSK &= ~0x80;
				}
				else
				{
					Init_check[TimerID]=NOT_INITIALIZED;
					retval = NOK;
				}

				break;

#endif
				/*
				 * In case of wrong timer ID
				 * return not initialized timer and NOT OK
				 */
				default:
					Init_check[TimerID]=NOT_INITIALIZED;
					retval = NOK;
	}


	return retval;
}



EnmTIMERError_t TIMER_start(uint8 TimerID)
{
	static uint8 funcStatus = OK;

	switch(TimerID)
	{
	case TIMER0:
		switch(TIMER_cnfg_arr[TimerID].Prescaler)
		{
		case NO_CLK_SRC:
		case F_CPU_1:
		case F_CPU_8:
		case F_CPU_64:
		case F_CPU_256:
		case F_CPU_1024:
		case EXT_CLK_FALLING:
		case EXT_CLK_RISING:
			TCCR0 |= TIMER_cnfg_arr[TimerID].Prescaler;
			break;
		default:

			Init_check[TimerID]=NOT_INITIALIZED;
			funcStatus = NOK;
		}


		break;

		case TIMER1:
			switch(TIMER_cnfg_arr[TimerID].Prescaler)
			{
			case NO_CLK_SRC:
				TCCR1B |= 0b00000000;
				break;
			case F_CPU_1:
				TCCR1B |= 0b00000001;
				break;
			case F_CPU_8:
				TCCR1B |= 0b00000010;
				break;
			case F_CPU_64:
				TCCR1B |= 0b00000011;
				break;
			case F_CPU_256:
				TCCR1B |= 0b00000100;
				break;
			case F_CPU_1024:
				TCCR1B |= 0b00000101;
				break;
			case EXT_CLK_FALLING:
				TCCR1B |= 0b00000110;
				break;
			case EXT_CLK_RISING:
				TCCR1B |= 0b00000111;
				break;
			default:
				Init_check[TimerID]=NOT_INITIALIZED;
				funcStatus = NOK;
				break;
			}

			break;

			case TIMER2:
				switch(TIMER_cnfg_arr[TimerID].Prescaler)
				{
				case F_CPU_64:
					TCCR2 |= TIMER_cnfg_arr[TimerID].Prescaler+1;
					break;

				case F_CPU_256:
				case F_CPU_1024:
					TCCR2 |= TIMER_cnfg_arr[TimerID].Prescaler+2;
					break;


				case NO_CLK_SRC:
				case F_CPU_1:
				case F_CPU_8:
					TCCR2 |= TIMER_cnfg_arr[TimerID].Prescaler;
					break;

				case F_CPU_32:
					TCCR2 |= 3;
					break;
				case F_CPU_128:
					TCCR2 |= 5;
					break;
				default:
					Init_check[TimerID]=NOT_INITIALIZED;
					funcStatus = NOK;
				}

				break;
	}

	return funcStatus;
}



EnmTIMERError_t TIMER_stop(uint8 TimerID)
{
	static uint8 funcStatus = OK;
	switch(TimerID)
	{
	case TIMER0:

		TCCR0 &= ~ 0x07;
		break;

	case TIMER1:

		TCCR1B &= ~ 0x07;
		break;

	case TIMER2:

		TCCR2 &= ~ 0x07;
		break;

	default :
		funcStatus = NOK;
		break;
	}


	return funcStatus;
}

EnmTIMERError_t Timer_Cal(uint8 tick){
	EnmTIMERError_t status =OK;
	float tcnt_val =0;
	float prescale = ((1/8000000)*1000);
	switch(TIMER_cnfg_arr[TimerID].TIMER_ID){
		case TIMER0 :
			break;
		case TIMER1 :
			tcnt_val = tick/prescale;
			TCNT1 = tcnt_val;
			TCCR1B |= 0b00000010;
			break;
		case TIMER2 :
			break;
		default :
			status = NOK;
			break;
	}
	return status;
}
/*EnmTIMERError_t TIME_Calc(Timer_ID TimerID,uint8 TIME, void (* ptrcallback)(void))
{
	static uint8 LOC_Func_Status=OK;
	static uint8 TICK_Time=0;
	static uint32 maxApplicableTickTime=0;
	***********************************************************************
	 * 				Calculating timer tick based on the prescaler
	 ***********************************************************************
	switch(TIMER_cnfg_arr[TimerID].Prescaler)
	{
	case F_CPU_1:
		TICK_Time= (1.0*1000000UL/ F_CPU);
		break;
	case F_CPU_8:
		TICK_Time= (8.0*1000000UL/ F_CPU);
		break;
	case F_CPU_32:
		TICK_Time= (32.0*1000000UL/ F_CPU);
		break;
	case F_CPU_64:
		TICK_Time= (64.0*1000000UL/ F_CPU);
		break;
	case F_CPU_128:
		TICK_Time= (128.0*1000000UL/ F_CPU);
		break;
	case F_CPU_256:
		TICK_Time= (256.0*1000000UL/ F_CPU);
		break;
	case F_CPU_1024:
		TICK_Time= (1024.0*1000000UL/ F_CPU);
		break;
	default:
		LOC_Func_Status=NOK;
		break;
	}

	***********************************************************************
	 * 			Calculating the interrupt time and set the call back
	 ***********************************************************************
	switch(TIMER_cnfg_arr[TimerID].OC_ChA_Mode)
	{
	case NORMAL_MODE:


		switch(TimerID)
		{
		case TIMER0:
			maxApplicableTickTime = TICK_Time*256;
			TIMER0_OVF_setCallBack(ptrcallback);
			break;

		case TIMER1:
			maxApplicableTickTime = TICK_Time*65536;
			TIMER1_OVF_setCallBack(ptrcallback);
			break;

		case TIMER2:
			maxApplicableTickTime = TICK_Time*256;
			TIMER2_OVF_setCallBack(ptrcallback);
			break;
		default:
			LOC_Func_Status=NOK;
			break;
		}

		break;

		case CTC_MODE:

			maxApplicableTickTime = TICK_Time*TIMER_cnfg_arr[TimerID].OCR_ChA;

			switch(TimerID)
			{
			case TIMER0:
				TIMER0_CMP_setCallBack(ptrcallback);
				break;

			case TIMER1:
				TIMER1_CMPA_setCallBack(ptrcallback);
				break;

			case TIMER2:
				TIMER2_CMP_setCallBack(ptrcallback);
				break;
			default:
				LOC_Func_Status=NOK;
				break;
			}

			break;
			default:
				LOC_Func_Status=NOK;
				break;
	}

	return LOC_Func_Status;
}*/


void TIMER0_OVF_setCallBack(void (*COPY_TIMER0_OVF_callBackPtr)(void))
{

	g_TIMER0_OVF_callBackPtr = COPY_TIMER0_OVF_callBackPtr;


}

void TIMER0_CMP_setCallBack(void (*COPY_TIMER0_CMP_callBackPtr)(void))
{
	g_TIMER0_CMP_callBackPtr = COPY_TIMER0_CMP_callBackPtr;
}

void TIMER1_OVF_setCallBack(void (*COPY_TIMER1_OVF_callBackPtr)(void))
{
	g_TIMER1_OVF_callBackPtr = COPY_TIMER1_OVF_callBackPtr;
}

void TIMER1_CMPA_setCallBack(void (*COPY_TIMER1_CMPA_callBackPtr)(void))
{
	g_TIMER1_CMPA_callBackPtr = COPY_TIMER1_CMPA_callBackPtr;
}

void TIMER1_CMPB_setCallBack(void (*COPY_TIMER1_CMPB_callBackPtr)(void))
{
	g_TIMER1_CMPB_callBackPtr = COPY_TIMER1_CMPB_callBackPtr;
}

void TIMER2_OVF_setCallBack(void (*COPY_TIMER2_OVF_callBackPtr)(void))
{
	g_TIMER2_OVF_callBackPtr = COPY_TIMER2_OVF_callBackPtr;
}

void TIMER2_CMP_setCallBack(void (*COPY_TIMER2_CMP_callBackPtr)(void))
{
	g_TIMER2_CMP_callBackPtr = COPY_TIMER2_CMP_callBackPtr;
}
