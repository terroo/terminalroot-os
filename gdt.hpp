/* 
   * @file - GDT (Global Descriptor Table) Header file.
   *
   * Engineer: João Guilherme da Silva Feitas Lima <joaoanandalima@gmail.com>
   * 
   * 
 */

#ifndef __GDT_HEADER__
#define __GDT_HEADER__ 

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

struct GDTEntry {
    uint16_t limit_low;     
    uint16_t base_low;      
    uint8_t  base_middle;   
    uint8_t  access;        
    uint8_t  granularity;   
    uint8_t  base_high;     
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;         
    uint32_t base;          
} __attribute__((packed));

#endif