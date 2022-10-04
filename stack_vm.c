#include "stack_vm.h"

int16_t memory[MEMORY_MAX];
uint16_t reg[3];

int push(int16_t n)
{
  if (reg[SP] <= STACK_END)
    return FULL_STACK_ERROR;
  memory[reg[SP]--] = n;
  return SUCCESS;
}

int pop(int16_t *n)
{
  if (reg[SP] >= STACK_START)
    return EMPTY_STACK_ERROR;
  *n = memory[++reg[SP]];
  return SUCCESS;
}

int pop2(int16_t *n, int16_t *m)
{
  if (reg[SP] >= (STACK_START - 1))
    return EMPTY_STACK_ERROR;
  *n = memory[++reg[SP]];
  *m = memory[++reg[SP]];
  return SUCCESS;
}

int peek(int16_t *n)
{
  if (reg[SP] >= STACK_START)
    return EMPTY_STACK_ERROR;
  *n = memory[reg[SP] + 1];
  return SUCCESS;
}

int swap(uint16_t n)
{
  if (n <= 0)
    return INVALID_ARGUMENT_ERROR;
  if (reg[SP] >= (STACK_START - n))
    return STACK_TOO_SMALL_ERROR;
  uint16_t i = reg[SP] + 1;
  uint16_t j = reg[SP] + (n + 1);
  uint16_t a = memory[i];
  memory[i] = memory[j];
  memory[j] = a;
  return SUCCESS;
}

int16_t sign_extend(uint16_t n, unsigned int num_bits)
{
  if ((n >> (num_bits - 1)) & 1)
    n |= (0xFFFF << num_bits);
  return n;
}

int one_arg_no_operand(int16_t (*f)(int16_t))
{
}

int two_args_no_operand(int16_t (*f)(int16_t, int16_t))
{
  int16_t a;
  int16_t b;
  int return_value = pop2(&a, &b);
  push(f(a, b));
  return return_value;
}

int handle_instruction(uint16_t instruction)
{
  unsigned int operation = instruction >> 13;
  unsigned int operand = instruction & ((1 << 13) - 1);
  switch (operation)
  {
  // Push an immediate value on the stack
  case OP_PUSH:
  {
    int16_t imm13 = sign_extend(operand, 13);
    return push(imm13);
  }

  // Load a value from memory onto the top of the stack
  case OP_LOAD:
  {
    uint16_t addr13 = operand;
    int16_t value = memory[addr13];
    return push(value);
  }

  // Pop the top value off of the stack and store in memory
  case OP_STOR:
  {
    uint16_t addr13 = operand;
    int16_t value;
    int return_value = pop(&value);
    memory[addr13] = value;
    return return_value;
  }

  // Moves the instruction pointer to the address specified by the immediate value
  case OP_JMP:
  {
    uint16_t addr13 = operand;
    reg[IP] = addr13;
    return SUCCESS;
  }

  // If the conditional flag is set to True, move the instruction pointer to the address specified by the immediate value
  case OP_BR:
  {
    uint16_t addr13 = operand;
    if (1)
    {
      reg[IP] = addr13;
    }
    return SUCCESS;
  }

  // Swaps the current TOS with the value n addresses above, where n is a 13-bit immediate
  case OP_SWAP:
  {
    uint16_t imm13 = operand;
    return swap(imm13);
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
      int16_t a;
      return pop(&a);
    }
    case NO_ADD:
    {
      return two_args_no_operand(f_add);
    }
    case NO_SUB:
    {
      return two_args_no_operand(f_subtract);
    }
    case NO_MULT:
    {
      return two_args_no_operand(f_multiply);
    }
    case NO_MULTC:
    {
      int16_t a;
      int16_t b;
      int return_value = pop2(&a, &b);
      if (return_value != SUCCESS)
        return return_value;
      int32_t result = a * b;
      int16_t hi_result = (int16_t)hi((uint32_t)result);
      int16_t lo_result = (int16_t)lo((uint32_t)result);
      push(hi_result);
      push(lo_result);
      return SUCCESS;
    }
    case NO_DIV:
    {
      return two_args_no_operand(f_divide);
    }
    case NO_MOD:
    {
      return two_args_no_operand(f_mod);
    }
    case NO_AND:
    {
      return two_args_no_operand(f_and);
    }
    case NO_OR:
    {
      return two_args_no_operand(f_or);
    }
    case NO_NOT:
    {
      return one_arg_no_operand(f_not);
    }
    case NO_SHFTL:
    {
      return two_args_no_operand(f_left_shift);
    }
    case NO_SHFTR:
    {
      return two_args_no_operand(f_right_shift);
    }
    case NO_DUP:
    {
      int16_t a;
      int return_value_1 = pop(&a);
      if (return_value_1 != SUCCESS)
        return return_value_1;
      int return_value_2 = push(a);
      if (return_value_2 != SUCCESS)
        return return_value_2;
      int return_value_3 = push(a);
      if (return_value_3 != SUCCESS)
        return return_value_3;
      return SUCCESS;
    }
    case NO_SWAPS:
    {
      uint16_t n;
      int return_value_1 = pop(&n);
      if (return_value_1 != SUCCESS)
        return return_value_1;
      return swap(n);
    }
    case NO_LOADS:
    {
    }
    case NO_LOADSI:
    {
    }
    case NO_STORS:
    {
    }
    case NO_STORSI:
    {
    }
    default:
    {
      return INVALID_INSTRUCTION_ERROR;
    }
    }
  }
  default:
  {
    return INVALID_INSTRUCTION_ERROR;
  }
  }
}

unsigned int program_length(FILE *file)
{
  fseek(file, 0, SEEK_END);
  unsigned int length = (unsigned int)ftell(file);
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