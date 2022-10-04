#include <stdio.h>
#include <stdint.h>
#include "stack_vm.h"

uint16_t memory[MEMORY_MAX];
uint16_t reg[3];

int test_stack_size(unsigned int n)
{
  uint16_t sp = reg[SP];
  if (sp - STACK_START < n)
  {
    return 0;
  }
  return 1;
}

void push(uint16_t n)
{
}

uint16_t pop(void)
{
}

uint16_t peek(void)
{
}

void handle_instruction(uint16_t instruction)
{
  unsigned int operation = instruction >> 13;
  unsigned int operand = instruction & ((1 << 13) - 1);
  switch (operation)
  {
  // Push an immediate value on the stack
  case OP_PUSH:
  {
    uint16_t addr13 = operand;
    push(addr13);
    break;
  }

  // Load a value from memory onto the top of the stack
  case OP_LOAD:
  {
    uint16_t addr13 = operand;
    uint16_t value = memory[addr13];
    push(value);
    break;
  }

  // Pop the top value off of the stack and store in memory
  case OP_STOR:
  {
    uint16_t addr13 = operand;
    uint16_t value = pop();
    memory[addr13] = value;
    break;
  }

  // Moves the instruction pointer to the address specified by the immediate value
  case OP_JMP:
  {
    uint16_t addr13 = operand;
    reg[IP] = addr13;
  }

  // If the conditional flag is set to True, move the instruction pointer to the address specified by the immediate value
  case OP_BR:
  {
    uint16_t addr13 = operand;
    if (1)
    {
      reg[IP] = addr13;
    }
  }

  // Swaps the current TOS with the value n addresses above, where n is a 13-bit immediate
  case OP_SWAP:
  {
    uint16_t imm13 = operand;
    uint16_t top = pop();
  }

  // Executes a trap code
  case OP_TRAP:
  {
    switch (operand)
    {
    case TRAP_GETC:
    {
    }

    case TRAP_PUTC:
    {
    }

    case TRAP_HALT:
    {
    }
    }
  }

  // Executes a no-operand instruction
  case OP_NO:
  {
    switch (operand)
    {
    case NO_POP:
    {
      pop();
      break;
    }
    case NO_ADD:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a + b);
      break;
    }
    case NO_SUB:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a - b);
      break;
    }
    case NO_MULT:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a * b);
      break;
    }
    case NO_MULTC:
    {
    }
    case NO_DIV:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a / b);
      break;
    }
    case NO_MOD:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a % b);
      break;
    }
    case NO_AND:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a & b);
      break;
    }
    case NO_OR:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a | b);
      break;
    }
    case NO_NOT:
    {
      uint16_t a = pop();
      push(~a);
      break;
    }
    case NO_SHFTL:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a << b);
      break;
    }
    case NO_SHFTR:
    {
      uint16_t a = pop();
      uint16_t b = pop();
      push(a >> b);
      break;
    }
    case NO_DUP:
    {
      uint16_t a = pop();
      push(a);
      push(a);
      break;
    }
    case NO_SWAPS:
    {
    }
    case NO_LOADS:
    {
    }
    case NO_LOADSI:
    {
    }
    }
  }
  }
}

int program_length(FILE *file)
{
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);
  return length;
}

void fetch_instructions(FILE *file, uint16_t *data, unsigned int length)
{
  size_t read = fread(data, sizeof(uint16_t), length, file);
}

// Main
int main(int argc, char *argv[])
{
  // Making sure file was supplied
  if (argc != 2)
    return 1;
  FILE *file;
  if ((file = fopen(argv[1], "rb")) == NULL)
  {
    fclose(file);
    return 1;
  }
  unsigned int file_length = program_length(file) / 2;
  printf("%d\n", file_length);
  fetch_instructions(file, memory + INSTRUCTION_START, file_length / 2);
  fclose(file);

  // Initalizing stack, instruction, and user memory pointers
  reg[SP] = STACK_START;
  reg[IP] = INSTRUCTION_START;

  // Main loop
  uint16_t instruction;
  while (1)
  {
    instruction = memory[reg[IP]];
  }
}