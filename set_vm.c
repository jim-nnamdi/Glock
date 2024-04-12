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
  fl_pos = 1 << 0,  /* pos flag */
  fl_zro = 1 << 1,  /* zero flag */
  fl_neg = 1 << 2   /* neg flag */
};

int main(char* argc, const char* argv[]) {
  /* at every given point in time there should be a flag */
  /* the conditional register of the processor handles it */
  reg[R_COND] = fl_zro;
  
  /* trap routines which replace Program counters take the */
  /* lower part of the memory address, so therefore the first */
  /* memory location of our processor would be 0x3000 */
  enum {PC_START = 0X3000};
  reg[R_PC] = PC_START;
}