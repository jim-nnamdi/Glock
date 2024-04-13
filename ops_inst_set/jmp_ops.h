#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void jmp_operation(uint16_t instr) {
  uint16_t r1 = (instr >> 6) & 0x7; /* general purpose */
  reg[R_PC] = reg[r1];
}