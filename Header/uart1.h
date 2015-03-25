/* 
 * File:   uart.h
 * Author: ingrasys8F
 *
 * Created on 2015?2?27?, ?? 4:21
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif


int Initialize_Uart(void);
void xmitc(char data);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

