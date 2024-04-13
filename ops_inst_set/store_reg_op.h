#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void store_register_operation(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t pc_offset = sign_extend(instr & 0x3F,6);
  mem_write(reg[r1] + pc_offset, reg[r0]); 
}