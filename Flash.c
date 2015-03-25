/*
 * File:   Flash.c
 * Author: JerryHsieh
 * Function: Flash for read and wriet flash
 */

#include <stdio.h>   //for NULL definition
#include <p24FJ128GB106.h>
#include "Header/pic24_generic.h"
#include <xc.h>



//doWriteLatch ;W0=TBLPAG,W1=Wn,W2=WordHi,W3=WordLo - no return values
void doWriteLatchFlash(uint16 u16_addrhi, uint16 u16_addrlo, uint16 u16_wordhi, uint16 u16_wordlo) {
   TBLPAG = u16_addrhi;
   __builtin_tblwtl(u16_addrlo,u16_wordlo);     //equivalent to asm("tblwtl W3,[W1]")
   __builtin_tblwth(u16_addrlo,u16_wordhi);     //equivalent to asm("tblwth W2,[W1]")
 }

 //_ReadLatch: ;W0=TBLPAG,W1=Wn - data in W1:W0
 uint32 doReadLatchFlash(uint16 u16_addrhi, uint16 u16_addrlo) {
   union32 u32_a;
   TBLPAG = u16_addrhi;
   u32_a.u16.ls16 = __builtin_tblrdl(u16_addrlo);   //equivalent to asm("tblrdl [W1],W0")
   u32_a.u16.ms16 = __builtin_tblrdh(u16_addrlo);   //equivalent to asm("tblrdl [W1],W1")
   return(u32_a.u32);
 }


void doErasePageFlash (uint16 u16_addrhi, uint16 u16_addrlo) {
   uint16 u16_save_SR, u16_save_TBLPAG;

 // preserve the SR and TBLPAG registers
   u16_save_SR = SR;
   u16_save_TBLPAG = TBLPAG;
 //disable interrupts
   SR = SR | 0xE0;
 // NVCON = flash write + erase + page erase
   NVMCON = 0x4042;
   TBLPAG = u16_addrhi; // select page
 //select row
 //equivalant to "tblwtl W1,[W1]"
   asm("tblwtl  %0,[%0]"::"r"(u16_addrlo));
 //start erase
   __builtin_write_NVM();
 //reenable interrupts
   SR  = u16_save_SR;
 //wait for end of erase
   while (NVMCON & 0x8000)
     //doHeartbeat();
 // restore TBLPAG
   TBLPAG = u16_save_TBLPAG;
 }

 void doWriteRowFlash() {
  uint16 u16_save_SR;
   // save SR
   u16_save_SR = SR;
   // disable interrupts
   SR = SR | 0xE0;
   // flash write + row op
   NVMCON = 0x4001;
 //start write
   __builtin_write_NVM();
 //reenable interrupts
   SR = u16_save_SR;
 //wait for end of write
   while (NVMCON & 0x8000);
     //doHeartbeat();
 }

 void doWritePageFlash(union32 u32_pmemAddress, uint8* pu8_data, uint16 u16_len) {
   uint16 u16_byteCnt;
   union32 u32_a;
   uint16 u16_ICnt, u16_numInstructions;

   //ASSERT(u16_len <= FLASH_PAGEBYTES);
   doErasePageFlash(u32_pmemAddress.u16.ms16, u32_pmemAddress.u16.ls16);  //erase page
   //write the bytes
   //round up to nearest row boundary
   u16_numInstructions = u16_len/3;
   if (u16_len % 3 != 0) u16_numInstructions++;
   u16_numInstructions += (u16_numInstructions%64);
   for (u16_ICnt = 0, u16_byteCnt=0;u16_ICnt<u16_numInstructions; u16_ICnt += 1,u16_byteCnt += 3) {
     u32_a.u8[0] = pu8_data[u16_byteCnt];
     u32_a.u8[1] = pu8_data[u16_byteCnt+1];
     u32_a.u8[2] = pu8_data[u16_byteCnt+2];
     u32_a.u8[3] = 0;
     doWriteLatchFlash(u32_pmemAddress.u16.ms16, u32_pmemAddress.u16.ls16,u32_a.u16.ms16,u32_a.u16.ls16);
     if ((u16_ICnt+1)%64 == 0) {
       //row boundary, write it.
       doWriteRowFlash();
     }
     u32_pmemAddress.u32 += 2;  //program memory address increments by 2
   }
 }

 void doReadPageFlash(union32 u32_pmemAddress, uint8* pu8_data, uint16 u16_len) {
   uint16 u16_byteCnt;
   union32 u32_a;

   //ASSERT(u16_len <= FLASH_PAGEBYTES);
   for (u16_byteCnt=0;u16_byteCnt<u16_len;u16_byteCnt += 3) {
     u32_a = (union32) doReadLatchFlash(u32_pmemAddress.u16.ms16, u32_pmemAddress.u16.ls16);
     pu8_data[u16_byteCnt] = u32_a.u8[0];
     pu8_data[u16_byteCnt+1] = u32_a.u8[1];
     pu8_data[u16_byteCnt+2] = u32_a.u8[2];
     u32_pmemAddress.u32 += 2;
   }
 }
