/*
 * Driver_UART.c
 * Implements the Driver Functions for UART
 * Implements the Ring Buffer to Store the Values from UART
 * Created: 1/29/2017 9:46:04 AM
 *  Author: Ram
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "Globals.h"
#include "Driver_UART.h"
Bool Check_BufferFull();
Bool Check_BufferEmpty();
Bool Ready2Read();
void Rollback_BufferPointer();
Bool Write_RecBuffer(uint8_t data);
uint8_t Read_RecBuffer();
void Console_Output(unsigned char transmit);
void Console_OutputBuffer();
int Head=0,Tail=0;
extern ErrorStatus;

//UART Init:
void USART_Init()											 //USART Initialize with 8bit: 1Stop bit: No Parity
{
	UCSR0B=(1<<TXEN0)|(1<<RXEN0);										//Without USART Interrupts enabled.
	//UCSR0C=(1<<URSEL);									// Reserved for Further Use
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);							// Baud rate 19.2Kbps
	UBRR0L=0x19;
	//sei();
}

// UART1 Used for Xbee Interface
 void USART_Receive_Init()									//USART Initialize with 8bit: 1Stop bit: No Parity
 {
	 UCSR1B=(1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1)|(1<<TXCIE1);							//With USART Interrupts enabled.
	 //UCSR0C=(1<<URSEL);									// Reserved for Further Use
	 UCSR1C=(1<<UCSZ01)|(1<<UCSZ00);
	 UBRR1L=0x0C;											// Baud rate 38.4kbps
	 sei();
 }

void USART_putstring(char* StringPtr)
 {
	 while(*StringPtr != 0x00)
	 {												//Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		 Console_Output(*StringPtr);				    //Using the simple send function we send one char at a time
		 StringPtr++;									//We increment the pointer so we can read the next char
	 }
	 Console_Output('\r');							// Carriage Return
 }
 
void USART_WriteData(uint8_t* data)
{
	 while(*data != NULL)
	 {												//Here we check if there is still more chars to send, this is done checking the actual char and see if it is different from the null char
		 Console_Output(*data);				    //Using the simple send function we send one char at a time
		 data++;									//We increment the pointer so we can read the next char
	 }
	 Console_Output('\r');							// Carriage Return
}

void Recvframe(uint8_t data)
{	
	Bool Status = Write_RecBuffer(data);
	//if (Status == FALSE)
	// Do Assert: Stating the Error: Buffer full :: Call Asser Function &  Defining Priority & Message"
}

uint8_t Read_DataBuffer()
{
	if (Check_BufferEmpty())
	return NULL;
	return Read_RecBuffer();
}

Bool Write_RecBuffer(uint8_t data)
{
	Bool flag=FALSE;
	if(!Check_BufferFull())
	{
		recvbuffer[Head]=data;
		Head++;
		flag=TRUE;
	}
	Rollback_BufferPointer();
	return flag;
}

uint8_t Read_RecBuffer()
{
	uint8_t buffdata=0;
	buffdata=recvbuffer[Tail];
	Tail++;
	Rollback_BufferPointer();
	return buffdata;
}

 void Console_Output(unsigned char transmit)
 {
	 while (!(UCSR0A&(1<<UDRE0)));
	 UDR0=transmit;
 }
 
void Console_OutputBuffer()
  {
	  for (uint8_t i=0;i<PacketSize;i++,Tail++)
		  Console_Output(temp_buffer[Tail]);
	Rollback_BufferPointer();
  }
 //region Helper Interrupt_Functions
 ISR(USART1_RX_vect)
 {
	if (!Check_BufferFull())
	{
		temp_buffer[Head]=UDR1;
		Console_Output(temp_buffer[Head]);
		Console_Output(Head);
		Console_Output(Tail);
		Head++;
	}
	else
	{
		ErrorStatus=ErrorStatus|0x01;						// Setting Bit0 ==> for Buffer Full!!
		Console_Output(ErrorStatus);
	}		
	Rollback_BufferPointer();
 }
 // endregion Helper Interrupt_Functions
 
// Region: Helper Functions:
Bool Check_BufferFull() {return ((Tail == ((Head+1)%REC_BUFFER_MAXSIZE)) ? TRUE : FALSE);}
Bool Check_BufferEmpty(){return ((Head == Tail) ? TRUE : FALSE);}
Bool Ready2Read()		{return ((abs(Tail-Head) >= PacketSize) ? TRUE : FALSE);}
void Rollback_BufferPointer()
{
	if (Head>=REC_BUFFER_MAXSIZE)
	Head=0;
	if(Tail>=REC_BUFFER_MAXSIZE)
	Tail=0;
}
// End Region: Helper Functions:
