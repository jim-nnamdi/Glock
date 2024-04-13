#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../commons/common_set.h"

void add_ops(uint16_t instr) {
  uint16_t r0 = (instr >> 12) & 0x7;
      uint16_t r1 = (instr >> 6) & 0x7;
      
      /* check for addressing modes */
      /* After shifting, the result is then bitwise ANDed with 0x1. 
      The value 0x1 in binary is 0000 0000 0000 0001, which has only 
      the least significant bit set. The bitwise AND operation will 
      thus zero out all bits of the result except for the least significant bit.*/
      uint16_t imm5_flag = (instr >> 5) & 0x1;
      if(imm5_flag) {
        /* perform sign extending here */
        uint16_t imm5 = sign_extend(instr & 0X1F, 5);
        reg[r0] = reg[r1] + imm5;
      } else {
        uint16_t r2 = (instr >> 5) & 0x7;
        reg[r0] = reg[r1] + reg[r2];
      }
      /* remember the initial flag was set to fl_zro */
      /* after each operation reset the flag data */
      update_flag(r0);
}