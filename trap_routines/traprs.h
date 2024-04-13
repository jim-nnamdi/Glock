#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void trap_puts() {
  /* one character per word */
  uint16_t* c = memory + reg[R_R0];
  while(*c) {
    putc((char)*c, stdout);
    ++c;
  }
  fflush(stdout);
}

void trap_out() {
  putc((char) reg[R_R0], stdout);
  fflush(stdout);
}

void trap_getc() {
  reg[R_R0] = (uint16_t) getchar();
  update_flag(R_R0);
}

void trap_in() {
  printf("type a character");
  char c = getchar();
  putc(c,stdout);
  fflush(stdout);

  reg[R_R0] = (uint16_t)c;
  update_flag(R_R0);
}

void trap_halt() {
  puts("Halt");
  fflush(stdout);
}

void trap_putsp(){
    /* one char per byte (two bytes per word)
       here we need to swap back to
       big endian format */
    uint16_t* c = memory + reg[R_R0];
    while (*c)
    {
        char char1 = (*c) & 0xFF;
        putc(char1, stdout);
        char char2 = (*c) >> 8;
        if (char2) putc(char2, stdout);
        ++c;
    }
    fflush(stdout);
}