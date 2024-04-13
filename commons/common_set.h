#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include <sys/termios.h>
#include <sys/time.h>

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

/* trap routines are specifically used 
   for accepting input data coming from
   the keyboard or any input device, also
   they consist of the lower memory bits
   in the processor
*/
enum {
  TRAP_GETC = 0x20,     /* get a character don't echo */
  TRAP_OUT = 0X21,      /* output a character */
  TRAP_PUTS = 0X22,     /* output a word string */
  TRAP_IN = 0X23,       /* get a character and echo */
  TRAP_PUTSP = 0X24,    /* output a byte string */
  TRAP_HALT = 0X25      /* halt the program */
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

/* memory mapped registers are special registers*/
/* not available in the normal registers table */
/* to read and write to them, you need to read */
/* and write to their memory locations */
enum {
  MR_KBSR = 0XFE00,   /* keyboard status */
  MR_KBDR = 0XFE02    /* keyboard data */
};

struct termios initial_terminal_data;

void disable_input_buffering() {
  /* disable canonical which supports buffering, hence 
     the program returns immediately instead of buffering
  */
 tcgetattr(STDIN_FILENO, &initial_terminal_data);
 struct termios new_terminal_data = initial_terminal_data; /* copy original*/
 new_terminal_data.c_lflag = ~ICANON & ~ECHO;
 tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal_data);
}

void restore_input_buffering() {
  tcsetattr(STDIN_FILENO, TCSANOW, &initial_terminal_data);
}

uint16_t check_key() {
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  
  return select(1, &readfds, NULL, NULL, &timeout) != 0; /* returns a non-zero value if a key is pressed */
}

uint16_t mem_read(uint16_t address) {
  if(address == MR_KBSR){
    if(check_key()){
      memory[MR_KBSR] = (1 << 15);
      memory[MR_KBDR] = getchar();
    }else {
      memory[MR_KBSR] = 0;
    }
  }
  return memory[address];
}

void mem_write(uint16_t address, int data){
  memory[address] = data;
}