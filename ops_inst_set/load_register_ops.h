#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void load_register_ins(uint16_t instr){
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t offset = sign_extend(instr & 0x3F,6);
  reg[r0] = mem_read(reg[r1] + offset);
  update_flag(r0);
}