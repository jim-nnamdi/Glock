#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "commons/common_set.h"

uint16_t bitwise_and_ops(
  uint16_t instr
) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t imm_reg_flag = (instr >> 5) & 0x1;
  if(imm_reg_flag) {
    uint16_t imm_reg = sign_extend(instr & 0x1FF, 5);
    reg[r0] = reg[r1] & imm_reg;
  }else {
    uint16_t r2 = (instr >> 5) & 0x7;
    reg[r0] = reg[r1] & reg[r2];
  }update_flag(r0);
}