#ifndef CPU_HH
#define CPU_HH
#include <stdio.h>
#include "../misc/types.h"
#include "../memory/mem.hh"

typedef struct CPU{
    word pc;        //Program Counter
    byte sp;
    byte ps;        //Processor Status

    byte A,X,Y;

    byte C:1;       //Status Flags
    byte D:1;
    byte Z:1;
    byte I:1;
    byte B:1;
    byte V:1;
    byte N:1;

    //Opcodes
    static constexpr byte
        INST_LDA_IM = 0XA9,
        INST_LDA_ZP = 0xA5,
        INST_LDA_ZPX = 0xB5,
        INST_JSR = 0x20;

    void LDASetStatus(){
        Z = (A==0);
        N = (A & 0b10000000) > 0; 
    }

    //Cpu functions
    byte fetchByte(u32 cycles,Mem& mem){
        byte data = mem[pc];
        printf("%x,%x\n",pc,data);
        pc++;
        cycles--;
        return data;
    }

    word fetchWord(u32 cycles, Mem& mem){
        //Little endian
        word data = mem[pc];
        pc++;
        cycles--;

        data |= (mem.data[pc] << 8);      //8 bit shift
        pc++;
        cycles-=2;

        return data;
    }

    byte readByte(u32 cycles,byte address,Mem& mem){
        byte data = mem.data[address];
        cycles--;
        return data;
    }

    void reset(Mem& mem){
        pc = 0xFFFC;
        sp = 0xFF;
        A = X = Y = 0;
        C = D = Z = I = B = V = N  = 0;
        mem.init();
    }

    void execute(u32 cycles,Mem& mem){
        while (cycles>0){
            byte inst = fetchByte(cycles,mem);
            switch (inst){
            case INST_LDA_IM:{
                byte value = fetchByte(cycles,mem);
                A = value;      //General Purpose Register
                LDASetStatus();
            }break;
            case INST_LDA_ZP:{
                byte zeroPageAddr = fetchByte(cycles,mem);
                A = readByte(cycles,zeroPageAddr,mem);
                LDASetStatus();
            }break;
            case INST_LDA_ZPX:{
                byte zeroPageAddr = fetchByte(cycles,mem);
                zeroPageAddr += X;
                cycles--;
                A = readByte(cycles,zeroPageAddr,mem);
                LDASetStatus();
            }break;
            case INST_JSR:{
                word subaddr = fetchWord(cycles,mem);
                mem.writeWord(pc-1,sp,cycles);
                sp-=2;
                pc = subaddr;
                cycles--;
            }break;
            case (0):{
                return;
            }break;
            default:{
                printf("err: unable to handle instruction %d", inst);
                return;
            }break;
            }
        }
    }
}CPU;
#endif