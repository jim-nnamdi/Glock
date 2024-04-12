#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* we have about 1 << 16 memory locations */
#define MEMORY_LOC_MAX 1 << 16
uint16_t memory[MEMORY_LOC_MAX];

enum {
  R_R0 = 0,
  R_R1,
  R_R2,
  R_R3,
  R_R4,
  R_R5,
  R_R6,
  R_R7,
  R_PC,
  R_COND,
  R_COUNT
};

/* 
  store the registers inside an array
  data handling would be done from the 
  registers from the memory via the IS 
 */
uint16_t reg[R_COUNT];

enum{
  OP_BR = 0,    /* branch */
  OP_ADD,       /* add */
  OP_LD,        /* load */
  OP_ST,        /* store */
  OP_JSR,       /* jump registers */
  OP_AND,       /* bitwise and */
  OP_LDR,       /* load register */
  OP_STR,       /* store register */
  OP_RTI,       /* unused */
  OP_NOT,       /* bitwise not */
  OP_LDI,       /* load indirect */
  OP_STI,       /* store indirect */
  OP_JMP,       /* jump */
  OP_RES,       /* reserved (unused) */
  OP_LEA,       /* load effective address */
  OP_TRAP       /* execute trap */
};

enum {
  FL_POS = 1 << 0,  /* pos flag */
  FL_ZRO = 1 << 1,  /* zero flag */
  FL_NEG = 1 << 2   /* neg flag */
};

void update_flag(uint16_t register_data) {
  if(reg[register_data] == 0) {
    reg[R_COND] = FL_ZRO;
  }else if(reg[register_data] >> 15) {
    reg[R_COND] = FL_NEG;
  }else{
    reg[R_COND] = FL_POS;
  }
}

uint16_t sign_extend(uint16_t iset, int bit_count) {
  if((iset >> (bit_count - 1))){
    iset |= (0xFFFF -1);
  }
  return iset;
}

int main(char* argc, const char* argv[]) {
  /* at every given point in time there should be a flag */
  /* the conditional register of the processor handles it */
  reg[R_COND] = FL_ZRO;
  
  /* trap routines which replace Program counters take the */
  /* lower part of the memory address, so therefore the first */
  /* memory location of our processor would be 0x3000 */
  enum {PC_START = 0X3000};
  reg[R_PC] = PC_START;
  
  /* there are multiple instruction mapped out operational codes */
  /* recall that each of the instructions are 16 bits each and the */
  /* opcode takes the 4 Least bits of the 16 bits data and the rem */
  /* data set are the parameters passed */
  int is_vm_running = 1; 
  while(is_vm_running) {
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op_code = instr << 16;
    switch(op_code) {

      /* this should handle the vm to process */
      /* memory mapped instructions that come*/
      /* as addition operations */
      case OP_ADD:
      uint16_t r0 = (instr >> 12) & 0x7;
      uint16_t r1 = (instr >> 6) & 0x7;
      
      /* check for addressing modes */
      /* After shifting, the result is then bitwise ANDed with 0x1. 
      The value 0x1 in binary is 0000 0000 0000 0001, which has only 
      the least significant bit set. The bitwise AND operation will 
      thus zero out all bits of the result except for the least significant bit.*/
      uint16_t imm5_flag = (instr >> 5) & 0x1;
      if(imm5_flag) {
        /* perform sign extending here */
        uint16_t imm5 = sign_extend(instr & 0X1F, 5);
        reg[r0] = reg[r1] + imm5;
      } else {
        uint16_t r2 = (instr >> 5) & 0x7;
        reg[r0] = reg[r1] + reg[r2];
      }
      /* remember the initial flag was set to fl_zro */
      /* after each operation reset the flag data */
      update_flag(r0);
      break;
      case OP_LD:
      break;
      case OP_ST:
      break;
      default:
      break;
    }
  }
}