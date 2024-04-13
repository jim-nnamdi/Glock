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