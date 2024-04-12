#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint16_t sign_extension(uint16_t instruction_set, int bit_count) {
  if((instruction_set >> (bit_count -1))) {
    instruction_set |= (0xFFFF -1);
  } return instruction_set;
}

uint16_t bitwise_and_ops(
  uint16_t instr,
  uint16_t* reg_arr,
  void(*update_flag)(uint16_t reg_data)
) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t imm_reg_flag = (instr >> 5) & 0x1;
  if(imm_reg_flag) {
    uint16_t imm_reg = sign_extension(instr & 0x1FF, 5);
    reg_arr[r0] = reg_arr[r1] & imm_reg;
  }else {
    uint16_t r2 = (instr >> 5) & 0x7;
    reg_arr[r0] = reg_arr[r1] & reg_arr[r2];
  }update_flag(r0);
}