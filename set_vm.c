#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ops_inst_set/and_ops.h"
#include "commons/common_set.h"

int main(char* argc, const char* argv[]) {
  /* at every given point in time there should be a flag */
  /* the conditional register of the processor handles it */
  reg[R_COND] = FL_ZRO;
  
  /* trap routines which replace Program counters take the */
  /* lower part of the memory address, so therefore the first */
  /* memory location of our processor would be 0x3000 */
  enum {PC_START = 0X3000};
  reg[R_PC] = PC_START;
  
  /* there are multiple instruction mapped out operational codes */
  /* recall that each of the instructions are 16 bits each and the */
  /* opcode takes the 4 Least bits of the 16 bits data and the rem */
  /* data set are the parameters passed */
  int is_vm_running = 1; 
  while(is_vm_running) {
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op_code = instr << 16;
    switch(op_code) {

      /* this should handle the vm to process */
      /* memory mapped instructions that come*/
      /* as addition operations */
      case OP_ADD:
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
      break;
      case OP_AND:
      /* now hopefully this is not an issue */
      /* parent update_flag takes r0 (instr >> 9) & 0x7 */
      /* here i'm passing reg[r0] to get the initial val from the register */
      bitwise_and_ops(instr,reg,update_flag(reg[r0]));
      break;
      case OP_LD:
      break;
      case OP_ST:
      break;
      default:
      break;
    }
  }
}