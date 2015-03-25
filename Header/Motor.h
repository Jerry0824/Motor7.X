#define FCY 16000000UL //16MHZ
#include <libpic30.h>

#define H_Step          41                  // Change every H direction Motor step
#define V_Step          21                  // Change every V direction Motor step
#define speed           10


// define as Output
#define HBlack		LATBbits.LATB0
#define HYellow		LATBbits.LATB1
#define HCoffee		LATBbits.LATB2
#define HBlue		LATBbits.LATB3
#define VBlack		LATBbits.LATB4
#define VYellow		LATBbits.LATB5
#define VCoffee 	LATBbits.LATB6
#define VBlue		LATBbits.LATB7

void Scan_Proc(void);
