/* 
 * File:   Flash.h
 * Author: ingrasys8F
 *
 * Created on 2015年3月2日, 下午 1:35
 */
#include "pic24_generic.h"

#ifndef FLASH_H
#define	FLASH_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PAGE_SIZE   1536     // refence datasheet
#define ROW_SIZE   192       // reference datasheet
void doWriteLatchFlash(uint16 u16_addrhi, uint16 u16_addrlo, uint16 u16_wordhi, uint16 u16_wordlo);
uint32 doReadLatchFlash(uint16 u16_addrhi, uint16 u16_addrlo);
void doReadPageFlash(union32 u32_pmemAddress, uint8* pu8_data, uint16 u16_len);
void doWritePageFlash(union32 u32_pmemAddress, uint8* pu8_data, uint16 u16_len);
//char value u32_a.u32;



#ifdef	__cplusplus
}
#endif

#endif	/* FLASH_H */

