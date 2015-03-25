/**********************************************************************
* 2009 Microchip Technology Inc.
*
* FileName:        	uart.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       	PIC24F
* Compiler:        	MPLAB C30 v3.11 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        	Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Jyoti Shrinivas	04/04/09	First version of source file - v1
* Harsha.J.M			Modifications with library - version v1.1
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code uses the PERIPHERAL LIBRARY support available with MPLAB C30 Compiler
* Code Tested on:
* EXPLORER 16 demo board with PIC24FJ256GB110 controller
* The Processor starts with the External Crystal (8 Mhz).
*
* Uses Tx pin for transmission and Rx pin for reception.
* Assings U2RX to pin RP10 using PPS feature
* Assings U2CTS to pin RP32 using PPS feature
* Assings U2TX to pin RP17 using PPS feature
* Assings U2RTS to pin RP31 using PPS feature
* Baud rate of 9600 is configred at 8MHz oscillator fequency
**********************************************************************/
#define USE_AND_OR
#include "p24fxxxx.h"
#include "uart.h"
#include "pps.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(__PIC24FJ256GB110__)
/*************** COFIGURATION **************************************
*	Watchdog Timer Disabled
*	Two Speed Start-up enabled
*	Oscillator Selection: HS oscillator ( 8MHz crystal on EXPLORER 16 Board )
*	Clock switching and clock monitor both enabled
********************************************************************/
_CONFIG1(FWDTEN_OFF & ICS_PGx2)
_CONFIG2(IESO_ON & FNOSC_PRI & FCKSM_CSECME & POSCMOD_HS & PLLDIV_DIV2 & IOL1WAY_OFF)
#endif


unsigned char Rxdata[10];
unsigned char DataAvailable=0;
unsigned char Txdata[] = "Microchip";

#define STDIN   0
#define STDOUT  1
#define STDERR  2
 #define LF   '\n'
 #define CR   '\r'

//#ifdef STDOUT_REDIRECT

void    mon_putc(char ch);

//*************** Interrupt Service routine for UART2 Transmission *************************************
void __attribute__ ((interrupt,no_auto_psv)) _U2TXInterrupt(void)
{	
  static unsigned int i=0;
  U2TX_Clear_Intr_Status_Bit;  						//clear the interrupt status of UART2 TX
  if(Txdata[i]!='\0')								//check for end of string
  {
   while(BusyUART2());								//wait till the UART is busy
   WriteUART2((unsigned int)Txdata[i++]);			//Transmit the data to hyper terminal of computer
  }
  else
  DisableIntU2TX; 								//disable the UART TX interrupt after end of complete transmission
 
}

//*************** Interrupt Service routine for UART2 reception *************************************
void __attribute__ ((interrupt,no_auto_psv)) _U2RXInterrupt(void)
{
// static unsigned int j=0;
// U2RX_Clear_Intr_Status_Bit;  						//clear the interrupt status of UART2 RX
//
//   while(!DataRdyUART2());							//wait for data reception on RX
//   Rxdata[j++] = ReadUART2();						//Read the data from UART buffer
//   if(j == 10)
//   DataAvailable=1;									//Set the DataAvailability flag after complete reception
 }
void xmitc(char data)
{
    while(BusyUART2());         // wait until transmit buffer is ready for data
    WriteUART2((unsigned int)data);
}
//void putch(unsigned char byte)					//??????Printf????
//{
//    if(byte == '\n')							//????
//    xmitc('\r');
//    xmitc(byte);
//}
void mon_putc(char c)
{
    xmitc(c); // printf() passes each character to write to _mon_putc(), which in turn passes each character to a custom output function
}
int __attribute__((__weak__, __section__(".libc")))
write(int handle, void * buffer, unsigned int len)
{
    int i = 0;
    switch (handle)
    {
        case STDOUT:
        case STDERR:    while (i < len)
                            mon_putc(((char*)buffer)[i++]);
                        break;
    }
    return (len);  // number of characters written
}

 
//********************************************************************************************** 
int Initialize_Uart(void)
{	

//********************** Assign UART 2 signals onto pins using PPS ***********************************
        //OSCCON = 0b01100011;
        //PPSUnLock();
	iPPSInput(IN_FN_PPS_U2RX,IN_PIN_PPS_RP10);		//Assing U2RX to pin RP10
	//iPPSInput(IN_FN_PPS_U2CTS,IN_PIN_PPS_RPI32);	//Assing U2CTS to pin RP32
	iPPSOutput(OUT_PIN_PPS_RP17,OUT_FN_PPS_U2TX);	//Assing U2TX to pin RP17
	//iPPSOutput(OUT_PIN_PPS_RP31,OUT_FN_PPS_U2RTS);	//Assing U2RTS to pin RP31
        //PPSLock();


    CloseUART2();  									//dsiable UART if enabled previously

/*********************** UART 2 Configuration ****************************************************
*
*		9600 baudrate
*		Low baud rate
*		8 bit transmission/reception
*		No parity bit
*		1 stop bit
************************************************************************************************/	
    /*Enable UART intruupts*/	    
    ConfigIntUART2(UART_RX_INT_DIS | UART_TX_INT_DIS);
    //ConfigIntUART2(UART_RX_INT_EN |UART_RX_INT_PR6 | UART_TX_INT_EN |UART_TX_INT_PR6);
    OpenUART2(UART_EN, UART_TX_ENABLE, 103);			//configure UART and enable it
    //Open2USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_EIGHT_BIT & USART_ASYNCH_MODE & USART_ADDEN_OFF, BAUDRG);
    //baud2USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_IDLE_RX_PIN_STATE_HIGH & BAUD_AUTO_OFF & BAUD_WAKEUP_OFF & BAUD_16_BIT_RATE & USART_RX_INT_ON);

    //IFS1bits.U2TXIF = 1;
   
     /*wait till data is transmitted and received*/    
//    while(!DataAvailable);
//    DataAvailable=0;
    //setbuf(stdout, NULL);
    return 0;
  
} 

