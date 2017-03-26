/*
 * Test_USART.c
 *
 * Created: 1/8/2017 4:24:10 PM
 *  Author: Ram
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "Globals.h"
#include "Driver_UART.h"
uint8_t ErrorStatus=0;
void ReportError();
int main(void)
{	
	Bool FlagRec;
	extern Head;
	extern Tail;
	uint8_t Display[100];
	USART_Init();										// USART0 For Logging/Debug ; Baud rate 19.2Kbps
	USART_Receive_Init();								// USART1 For Xbee Interface ; Baud rate 38.4kbps
	DDRA=0XFF;
	char BuidVersion[]="Build Version: 0.1";
	_delay_ms(2000);                             //2secs Delay
	for (unsigned char i=0;i<5;i++)
	{
		PORTA=1<<PA0;
		_delay_ms(500);
		PORTA=0<<PA0;
		_delay_ms(500);
	}
	//USART_putstring(BuidVersion);
	// First Check Buffer is Empty, Else Report Error:
	if (!Check_BufferEmpty())
		ErrorStatus=ErrorStatus|0x02;							// Setting Bit1 ==> Error Buffer Not Empty
			
	while(1)
	{
		//Check for Any Errors:
		ReportError();
		FlagRec =Ready2Read();
		if(FlagRec == TRUE)
		{
			Console_Output(Ready2Read());
			Console_OutputBuffer();
			Console_Output(Head);
			Console_Output(Tail);
		}
		_delay_us(1);                            //1 micro secs Delay
	}
}
void ReportError()
{
	switch(ErrorStatus)
	{

	}
}

