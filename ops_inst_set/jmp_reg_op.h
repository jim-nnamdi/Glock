#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void jmp_register_operation(uint16_t instr) {
  uint16_t long_flag = (instr >> 11) & 0x7;
  reg[R_R7] = reg[R_PC];
  if(long_flag == 0) {
    uint16_t r0 = (instr >> 6) & 0x7;
    reg[R_PC] = reg[r0];
  }else {
    uint16_t offset = sign_extend(instr & 0x7FF, 11);
    reg[R_PC] += offset;
  }
}