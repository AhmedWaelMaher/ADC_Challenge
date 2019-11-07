/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Ahmed Wael
 *
 *******************************************************************************/

#include "lcd.h"
#include "DIO.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void LCD_init(void)
{
	DIO_Init();
#if (DATA_BITS_MODE == 4)
#ifdef UPPER_PORT_PINS
	//LCD_DATA_PORT_DIR |= 0xF0; /* Configure the highest 4 bits of the data port as output pins */
#else
	//LCD_DATA_PORT_DIR |= 0x0F; /* Configure the lowest 4 bits of the data port as output pins */
#endif
	LCD_sendCommand(FOUR_BITS_DATA_MODE); /* initialize LCD in 4-bit mode */
	LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE); /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
#elif (DATA_BITS_MODE == 8)
	/*LCD_DATA_PORT_DIR = 0xFF;  Configure the data port as output port
	LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE);  use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
#endif
	LCD_sendCommand(CURSOR_OFF); /* cursor off */
	LCD_sendCommand(CLEAR_COMMAND); /* clear LCD at the beginning */
}

void LCD_sendCommand(uint8 command)
{
	DIO_Write(LCD_RS,LOW);
	DIO_Write(LCD_RW,LOW);
	_delay_ms(1); /* delay for processing Tas = 50ns */
	DIO_Write(LCD_EN,HIGH);
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
#if (DATA_BITS_MODE == 4)
	/* out the highest 4 bits of the required command to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	DIO_Write(LCD_D4 , (command >> 4) &1);
	DIO_Write(LCD_D5 , (command >> 5) &1);
	DIO_Write(LCD_D6 , (command >> 6) &1);
	DIO_Write(LCD_D7 , (command >> 7) &1);
#else
	/*	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((command & 0xF0) >> 4);*/
#endif
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	DIO_Write(LCD_EN,LOW);
	_delay_ms(1); /* delay for processing Th = 13ns */
	DIO_Write(LCD_EN,HIGH);
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	DIO_Write(LCD_D4 , (command >> 0) &1);
	DIO_Write(LCD_D5 , (command >> 1) &1);
	DIO_Write(LCD_D6 , (command >> 2) &1);
	DIO_Write(LCD_D7 , (command >> 3) &1);
#else
	/*	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);*/
#endif
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	DIO_Write(LCD_EN,LOW);
	_delay_ms(1); /* delay for processing Th = 13ns */
#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT = command; /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
#endif
}

void LCD_displayCharacter(uint8 data)
{

	DIO_Write(LCD_RS,HIGH);
	DIO_Write(LCD_RW,LOW);
	_delay_ms(1); /* delay for processing Tas = 50ns*/
	DIO_Write(LCD_EN,HIGH);
	_delay_ms(1);  /*delay for processing Tpw - Tdws = 190ns*/
#if (DATA_BITS_MODE == 4)
	 /*out the highest 4 bits of the required data to the data bus D4 --> D7*/
	DIO_Write(LCD_D4 , (data >> 4) &1);
	DIO_Write(LCD_D5 , (data >> 5) &1);
	DIO_Write(LCD_D6 , (data >> 6) &1);
	DIO_Write(LCD_D7 , (data >> 7) &1);
#elif (DATA_BITS_MODE == 8)
	 /*code*/
#endif
	_delay_ms(1);  /*delay for processing Tdsw = 100ns*/
	DIO_Write(LCD_EN,LOW);
	_delay_ms(1);  /*delay for processing Th = 13ns*/
	DIO_Write(LCD_EN,HIGH);
	_delay_ms(1);  /*delay for processing Tpw - Tdws = 190ns
	 out the lowest 4 bits of the required data to the data bus D4 --> D7*/
	DIO_Write(LCD_D4 , (data >> 0) &1);
	DIO_Write(LCD_D5 , (data >> 1) &1);
	DIO_Write(LCD_D6 , (data >> 2) &1);
	DIO_Write(LCD_D7 , (data >> 3) &1);
	_delay_ms(1);  /*delay for processing Tdsw = 100ns*/
	DIO_Write(LCD_EN,LOW);
	_delay_ms(1);  /*delay for processing Th = 13ns*/
}

void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
	/***************** Another Method ***********************
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}
	 *********************************************************/
}

void LCD_goToRowCol(uint8 row,uint8 col)
{
	uint8 Address =0;

	/* first of all calculate the required address */
	switch(row)
	{
	case 0:
		Address=col;
		break;
	case 1:
		Address=col+0x40;
		break;
	case 2:
		Address=col+0x10;
		break;
	case 3:
		Address=col+0x50;
		break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION);
}

void LCD_displayStringRowCol(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowCol(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

void LCD_intgerToString(int data)
{
	char buff[16]; /* String to hold the ascii result */
	itoa(data,buff,10); /* 10 for decimal */
	LCD_displayString(buff);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); //clear display
}

