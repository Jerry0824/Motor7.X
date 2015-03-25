/*
 * File:   Main.c
 * Author: JerryHsieh
 * Function: Main code for Motor function by PIC24FJ128GB106 platform
 */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>
#include <p24FJ128GB106.h>
#include "Header/Motor.h"
#include "Header/uart1.h"
#include "Header/Flash.h"
#include "Header/Main.h"

int function_item = 0;
#define LAST_ITEM 1;

_CONFIG1 (0x3E7F)
_CONFIG2 (0x13B6)
_CONFIG3 (0x2E00)




//初始化與宣告CPU的各個IO腳位//
void Initialize_CPU(void) {

    // Set internal oscillator frequency//
    //CLKDIV = 0x0100;

    // Set GPIO to Intput/Output
    TRISBbits.TRISB0 = 0;       // Set HBlack to Output
    TRISBbits.TRISB1 = 0;       // Set HYellow to Output
    TRISBbits.TRISB2 = 0;       // Set HCoffee to Output
    TRISBbits.TRISB3 = 0;       // Set HBlue to Output
    TRISBbits.TRISB4 = 0;       // Set VBlack to Output
    TRISBbits.TRISB5 = 0;       // Set VYellow to Output
    TRISBbits.TRISB6 = 0;       // Set VCoffee to Output
    TRISBbits.TRISB7 = 0;       // Set VBlue to Output
}



int main(void) {
    Initialize_CPU();    
    Initialize_Uart();
    __delay_ms(2000);
    Scan_Proc();
    while(1){
    }
   
//    while (1)
//    {
//        if(function_item ++ > LAST_ITEM)
//            function_item = 0;
//        switch (function_item)
//        {
//            case 0:
//                Scan_Proc();
//                break;
//            default:
//                break;
//        }
//    }
}
