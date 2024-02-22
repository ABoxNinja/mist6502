#include <stdio.h>
#include <stdlib.h>
#include "cpu.hh"
#include "../memory/mem.hh"
#include "../misc/types.h"

int main(){
    CPU cpu;
    Mem mem;

    cpu.reset(mem);

    mem[0xFFFC] = CPU::INST_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INST_LDA_IM;
    mem[0x4243] = 0x84;
    
    cpu.execute(9,mem);

    return 0;
}