/*
 * Driver_UART.h
 *
 * Created: 1/29/2017 9:45:43 AM
 *  Author: Ram
 */ 


#ifndef DRIVER_UART_H_
#define DRIVER_UART_H_
// Defining Functions:

void USART_Init();	
void USART_putstring(char* StringPtr);
void USART_Receive_Init();
void USART_WriteData(uint8_t* data);
void Recvframe(uint8_t data);			// Write in to the Buffer of Size 2^8
uint8_t Read_DataBuffer();			// Read from the Buffer
#define PacketSize 20

#endif /* DRIVER_UART_H_ */