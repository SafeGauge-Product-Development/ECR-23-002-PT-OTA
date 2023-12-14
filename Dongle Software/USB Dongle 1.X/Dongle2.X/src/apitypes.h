
#ifndef APITYPES_H_
#define APITYPES_H_

typedef signed char         int8;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef signed short        int16;
typedef signed long         int32;
typedef unsigned long       uint32;
typedef signed long long    int64;
typedef unsigned long long  uint64;


typedef struct bd_addr_t
{
    uint8 addr[6];

}bd_addr;

typedef bd_addr hwaddr;
typedef struct
{
    uint8 len;
    uint8 data[];
}uint8array;

typedef struct
{
    uint8 len;
    int8 data[];
}string;


#endif
