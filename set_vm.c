#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ops_inst_set/add_op.h"
#include "ops_inst_set/and_ops.h"
#include "ops_inst_set/br__ops.h"
#include "ops_inst_set/load_indirect_ops.h"
#include "ops_inst_set/load_ops.h"
#include "ops_inst_set/load_register_ops.h"
#include "ops_inst_set/jmp_reg_op.h"
#include "ops_inst_set/jmp_ops.h"
#include "ops_inst_set/load_effective_address.h"
#include "ops_inst_set/not_op.h"
#include "ops_inst_set/store_op.h"
#include "ops_inst_set/store_indirect_op.h"
#include "ops_inst_set/store_reg_op.h"

#include "trap_routines/traprs.h"

uint16_t swap16(uint16_t x);

void read_image_file(FILE* file)
{
    /* the origin tells us where in memory to place the image */
    uint16_t origin;
    fread(&origin, sizeof(origin), 1, file);
    origin = swap16(origin);

    /* we know the maximum file size so we only need one fread */
    uint16_t max_read = MEMORY_LOC_MAX - origin;
    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);

    /* swap to little endian */
    while (read-- > 0)
    {
        *p = swap16(*p);
        ++p;
    }
}

uint16_t swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}

int read_image(const char* image_path)
{
    FILE* file = fopen(image_path, "rb");
    if (!file) { return 0; };
    read_image_file(file);
    fclose(file);
    return 1;
}

void handle_interrupt(int signal) {
  restore_input_buffering();
  printf("\n");
  exit(2);
}

int main(int argc, const char* argv[]) {
  signal(SIGINT,handle_interrupt);
  disable_input_buffering();

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
      add_ops(instr);
      break;
      case OP_AND:
      /* now hopefully this is not an issue */
      /* parent update_flag takes r0 (instr >> 9) & 0x7 */
      /* here i'm passing reg[r0] to get the initial val from the register */
      bitwise_and_ops(instr);
      break;
      case OP_BR:
      bitwise_br_ops(instr);
      break;
      case OP_LD:
      load_operation(instr);
      break;
      case OP_LDR:
      load_register_ins(instr);
      break;
      case OP_LDI:
      load_indirect_register(instr);
      break;
      case OP_JMP:
      jmp_operation(instr);
      break;
      case OP_JSR:
      jmp_register_operation(instr);
      break;
      case OP_LEA:
      load_effective_addr(instr);
      break;
      case OP_NOT:
      bitwise_not_operation(instr);
      break;
      case OP_ST:
      store_operation(instr);
      break;
      case OP_STR:
      store_register_operation(instr);
      break;
      case OP_STI:
      store_indirect_operation(instr);
      break;
      default:
      break;
    } 

    reg[R_R7] = reg[R_PC];
    switch(instr & 0xFF) {
      case TRAP_GETC:
        trap_getc();
        break;
    case TRAP_OUT:
        trap_out();
        break;
    case TRAP_PUTS:
        trap_puts();
        break;
    case TRAP_IN:
        trap_in();
        break;
    case TRAP_PUTSP:
      trap_putsp();
        break;
    case TRAP_HALT:
        trap_halt();
        break;
    }
  }
}