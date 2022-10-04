#ifndef STACK_VM_H
#define STACK_VM_H

#include <stdio.h>
#include <stdint.h>

#define MEMORY_MAX (1 << 16)
#define INSTRUCTION_START 0
#define STACK_START ((1 << 16) - 1)
#define STACK_END (1 << 15)

enum instructions
{
  OP_PUSH = 0,
  OP_LOAD,
  OP_STOR,
  OP_SWAP,
  OP_JMP,
  OP_BR,
  OP_NO,
  OP_TRAP
};

enum no_operation_instructions
{
  NO_POP = 0,
  NO_ADD,
  NO_SUB,
  NO_MULT,
  NO_MULTC,
  NO_DIV,
  NO_MOD,
  NO_AND,
  NO_OR,
  NO_NOT,
  NO_SHFTL,
  NO_SHFTR,
  NO_DUP,
  NO_SWAPS,
  NO_IP,
  NO_LOADS,
  NO_LOADSI,
  NO_STORS,
  NO_STORSI
};

enum trap_codes
{
  TRAP_GETC = 0,
  TRAP_PUTC,
  TRAP_HALT
};

enum registers
{
  IP = 0,
  SP,
  BP
};

int test_stack_size(unsigned int n);

void push(uint16_t n);

uint16_t pop(void);

uint16_t peek(void);

void handle_instruction(uint16_t instruction);

int program_length(FILE *file, uint16_t *data, unsigned int length);

void fetch_instructions(FILE *file, uint16_t *data, unsigned int length);

#endif