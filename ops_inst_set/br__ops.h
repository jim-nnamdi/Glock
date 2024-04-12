#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "commons/common_set.h"

uint16_t bitwise_br_ops(
  uint16_t instr
) {
  uint16_t cond_flag = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr >> 0x1FF, 9);
 if(cond_flag & reg[R_COND]) {
  reg[R_PC] += pc_offset;
 } 
}