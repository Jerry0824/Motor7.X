/*
 * File:   Motor.c
 * Author: JerryHsieh
 * Function: Motor function
 */

#include <stdio.h>
#include "Header/Motor.h"
#include <p24FJ128GB106.h>
#include "Header/Flash.h"
#include "Header/Main.h"

struct Interface Interfacetab[]={{1,1},{2,2},{3,3},{4,4},{5,5},{6,6}};

void H_Stop(void)
    {
        HBlack = 0;
        HYellow = 0;
        HCoffee = 0;
        HBlue = 0;
    }

void V_Stop(void)
    {
        VBlack = 0;
        VYellow = 0;
        VCoffee = 0;
        VBlue = 0;
    }

void Flash_SaveInt_H(int value)
{
    /* 1 page = 512 instruction = 1024 words
       1024(10進位) = 0x400(16進位)，所以Address起始位置要是0x400的倍數*/
     union32 address;
     address.u32 = 0x2400;                          //0x400的倍數
     unsigned char data[ROW_SIZE];
     memcpy(data, &value, sizeof(value));             // 將int拆成4個Byte存進Array裡面，一個Array[]容量為1個Byte。
     doWritePageFlash(address, data, sizeof(data));
}

void Flash_SaveInt_V(int value)
{
    /* 1 page = 512 instruction = 1024 words
       1024(10進位) = 0x400(16進位)，所以Address起始位置要是0x400的倍數*/
     union32 address;
     address.u32 = 0x2800;                          //0x400的倍數
     unsigned char data[ROW_SIZE];
     memcpy(data, &value, sizeof(value));             // 將int拆成4個Byte存進Array裡面，一個Array[]容量為1個Byte。
     doWritePageFlash(address, data, sizeof(data));
}

int Flash_LoadInt_H()
{
     int result;
     union32 address;
     address.u32 = 0x2400;
     doReadPageFlash(address, &result, sizeof(result));
     return result;
}

int Flash_LoadInt_V()
{
     int result;
     union32 address;
     address.u32 = 0x2800;
     doReadPageFlash(address, &result, sizeof(result));
     return result;
}

void HFW_Control(void)
{
    int i;
    for(i = 0; i < H_Step; i++){
        HBlack =1;
        HYellow = 0;
        HCoffee = 0;
        HBlue = 0;
        __delay_ms(speed);
        HBlack = 0;
        HYellow = 1;
        HCoffee = 0;
        HBlue = 0;
        __delay_ms(speed);
    }
     H_Stop();
}

void VFW_Control(void)
{
    int i;
    for(i = 0; i < V_Step; i++){
        VBlack = 0;
        VYellow = 1;
        VCoffee = 0;
        VBlue = 0; 
        __delay_ms(speed);
        VBlack = 1;
        VYellow = 0;
        VCoffee = 0;
        VBlue = 0;
        __delay_ms(speed);
    }
     V_Stop();
}

void HRW_Control(void)
{
    int i;
    for(i = 0; i < H_Step; i++){
        HBlack =0;
        HYellow = 0;
        HCoffee = 1;
        HBlue = 0;
        __delay_ms(speed);
        HBlack = 0;
        HYellow = 0;
        HCoffee = 0;
        HBlue = 1;
        __delay_ms(speed);
    }
     H_Stop();
}

void VRW_Control(void)
{
    int i;
    for(i = 0; i < V_Step; i++){
        VBlack = 0;
        VYellow = 0;
        VCoffee = 0;
        VBlue = 1; 
        __delay_ms(speed);
        VBlack = 0;
        VYellow = 0;
        VCoffee = 1;
        VBlue = 0;
        __delay_ms(speed);
    }
     V_Stop();
}



//-----------歸零功能-----------------
void Back_to_start(void) {
    printf("Back_to_start\n");
    int n = Flash_LoadInt_H(),i = Flash_LoadInt_V();
    printf("n=%d\ni=%d\n", n,i);

    for (n; n >= 1; n--) {
        HRW_Control();
        printf("IntH[%d]\n", n);
    }
    for (i; i >= 1; i--) {
        HRW_Control();
        printf("IntV[%d]\n", i);
    }
}

//*********依據移動方向，移動步數操作移動**********
void rotation ( int H_rotation, int H_direction , int V_rotation , int V_direction)
{
    int i;
    if (H_direction < 0)
     {
        for (i = 0; i < H_rotation; i++) {
        HRW_Control();
        printf("HRW[%d]\n",i);
        printf("result HRW = %d\n", Flash_LoadInt_H());
         }
     }
     else{
        for (i = 0; i < H_rotation; i++) {
        HFW_Control();
        printf("HFW[%d]\n",i);
        printf("result HFW = %d\n", Flash_LoadInt_H());
         }
     }

    if (V_direction < 0)
     {
        for (i = 0; i < V_rotation; i++) {
        VRW_Control();
        printf("VRW[%d]\n",i);
        printf("result VRW = %d\n", Flash_LoadInt_V());
         }
     }
     else{
        for (i = 0; i < V_rotation; i++) {
        VFW_Control();
        printf("VFW[%d]\n",i);
        printf("result VFW = %d\n", Flash_LoadInt_H());
         }
     }
}

//*********判定轉動方向，轉動步數**********
void Location(int value_H,int value_V,int Staytime)
{
    printf("LO_start\n");
    int a,b,c,d,H_rotation,H_direction,V_rotation,V_direction;
    a = value_H;
    b = Flash_LoadInt_H();
    c = value_V;
    d = Flash_LoadInt_V();

     if ((a-b) < 0)
     {
        H_rotation = b-a;
        H_direction = -1;
     }
     else{
        H_rotation = a-b;
        H_direction = 1;
         }

     if ((c-d) < 0)
     {
        V_rotation = d-c;
        V_direction = -1;
     }
     else{
        V_rotation = c-d;
        V_direction = 1;
         }

    rotation (H_rotation, H_direction, V_rotation, V_direction);
    Flash_SaveInt_H(a);
    Flash_SaveInt_V(c);
    __delay_ms(Staytime);
}



void Scan_Proc(void) {

     Back_to_start();     //歸零
     Flash_SaveInt_H(0);    //設定H初始值
     Flash_SaveInt_V(0);    //設定V初始值

     printf("Start\n");
     Location(Interfacetab[x1].x,Interfacetab[y1].y,2000);
     printf("Stop\n");

}