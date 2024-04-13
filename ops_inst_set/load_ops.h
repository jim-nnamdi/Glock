#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void load_operation(
  uint16_t instr
) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t pc_offset = sign_extend(instr >> 0x1F, 9);
  mem_read(reg[R_PC] + pc_offset);
  update_flag(r0);
}