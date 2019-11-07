/******************************************************************************
 *
 * Module: DIO
 *
 * File Name: Dio_confg.c
 *
 * Description: Dio configuration source file
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "Dio_confg.h"

/*Configure your pins with the same order in the enum
 { port , pin Number , Pin direction , output_value , Pullup/external resistor , initialized/not initialized }*/

DIO_Conf_t DIO_Array[NUM_PINS] = {

		{PORT_A, PIN4, OUTPUT, LOW, NA, INITIALIZED },
		{PORT_A, PIN5, OUTPUT, LOW, NA, INITIALIZED },
		{PORT_A, PIN6, OUTPUT, LOW, NA, INITIALIZED },
		{PORT_A, PIN7, OUTPUT, LOW, NA, INITIALIZED },

		{PORT_A, PIN3, OUTPUT, LOW, NA, INITIALIZED },
		{PORT_A, PIN1, OUTPUT, LOW, NA, INITIALIZED },
		{PORT_A, PIN2, OUTPUT, LOW, NA, INITIALIZED },

		{BUZZER, PIN3, OUTPUT, LOW, NA, INITIALIZED },
		{LED, PIN4, OUTPUT, LOW, NA, INITIALIZED }
};
