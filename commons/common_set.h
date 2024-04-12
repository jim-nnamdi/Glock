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

void* update_flag(uint16_t register_data) {
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