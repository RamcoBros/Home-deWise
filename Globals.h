/*
 * Globals.h
 *
 * Created: 1/16/2017 8:46:59 PM
 *  Author: Ram
 */ 


#ifndef GLOBALS_H_
#define GLOBALS_H_

#ifndef REC_BUFFER_MAXSIZE
/* prevent compiler error by supplying a default */
# warning "F_CPU not defined for <util/delay.h>"
#define REC_BUFFER_MAXSIZE 256									// It should be 256 (2^8) Size of the Buffer!!!
#define Size (REC_BUFFER_MAXSIZE-1)
#endif

typedef char Bool;
#define TRUE 1
#define FALSE 0

uint8_t recvbuffer[REC_BUFFER_MAXSIZE],temp_buffer[100];
#endif /* GLOBALS_H_ */