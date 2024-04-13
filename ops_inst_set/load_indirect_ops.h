#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

/* load indirect is like having a pointer to a pointer */
/* so to access the underlying value, we need to read */
/* the memory twice */
void load_indirect_register(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t offset = sign_extend(instr & 0x1FF, 9);
  reg[r0] = mem_read(mem_read(reg[R_PC] + offset));
  update_flag(r0);
}