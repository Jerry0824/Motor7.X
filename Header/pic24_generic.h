typedef unsigned char       uint8;   //8 bits
typedef unsigned short      uint16;  //16 bits
typedef unsigned long       uint32;  //32 bits
typedef unsigned long long  uint64;  //64 bits

typedef signed char         int8;    //8 bits
typedef signed short        int16;   //16 bits
typedef signed long         int32;   //32 bits
typedef signed long long    int64;   //64 bits

typedef union _union16 {
   uint16 u16;
   uint8 u8[2];
 } union16;

typedef union _union32 {
   uint32 u32;

   struct {
     uint16 ls16;
     uint16 ms16;
   } u16;

   uint8 u8[4];
 } union32;

typedef union _union64 {
   uint32 u32[2];
   uint16 u16[4];
   uint8 u8[8];
 } union64;
