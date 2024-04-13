#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void load_effective_addr(uint16_t instr) {
  uint16_t dest_reg = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  reg[dest_reg] = reg[R_PC] + pc_offset;
  update_flag(dest_reg);
}