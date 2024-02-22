#ifndef MEM_HH
#define MEM_HH
#include "../misc/types.h"
static const u32 max_mem = 1024*64;

typedef struct{
    byte data[max_mem];

    void init(){
        for (u32 i = 0; i<max_mem; i++){
            data[i] = 0;
        }
    }
    byte operator[](u32 address) const{
        return data[address];
    }
    byte& operator[](u32 address){
        return data[address];
    }

    void writeWord(word value,u32 addr,u32 cycles){
        data[addr] = value & 0xFF;
        data[addr+1] = (value >> 8);
        cycles -= 2;
    }
}Mem;
#endif