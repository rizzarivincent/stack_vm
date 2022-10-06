#include "stack_vm.h"

struct StackVM init_stack_vm(struct StackVM *vm)
{
  vm->reg[IP] = INSTRUCTION_START;
  vm->reg[SP] = STACK_START;
}

int push(struct StackVM *vm, int16_t n)
{
  if (vm->reg[SP] <= STACK_END)
    return FULL_STACK_ERROR;
  vm->memory[vm->reg[SP]--] = n;
  return SUCCESS;
}

int pop(struct StackVM *vm, int16_t *n)
{
  if (vm->reg[SP] >= STACK_START)
    return EMPTY_STACK_ERROR;
  *n = vm->memory[++vm->reg[SP]];
  return SUCCESS;
}

int pop2(struct StackVM *vm, struct Pair *p)
{
  if (vm->reg[SP] >= (STACK_START - 1))
    return EMPTY_STACK_ERROR;
  p->a = vm->memory[++vm->reg[SP]];
  p->b = vm->memory[++vm->reg[SP]];
  return SUCCESS;
}

int peek(struct StackVM *vm, int16_t *n)
{
  if (vm->reg[SP] >= STACK_START)
    return EMPTY_STACK_ERROR;
  *n = vm->memory[vm->reg[SP] + 1];
  return SUCCESS;
}

int peek2(struct StackVM *vm, struct Pair *p)
{
  if (vm->reg[SP] >= (STACK_START - 1))
    return EMPTY_STACK_ERROR;
  p->a = vm->memory[vm->reg[SP] + 1];
  p->b = vm->memory[vm->reg[SP] + 2];
  return SUCCESS;
}

int swap(struct StackVM *vm, uint16_t n)
{
  if (n <= 0)
    return INVALID_ARGUMENT_ERROR;
  if (vm->reg[SP] >= (STACK_START - n))
    return STACK_TOO_SMALL_ERROR;
  uint16_t i = vm->reg[SP] + 1;
  uint16_t j = vm->reg[SP] + (n + 1);
  uint16_t a = vm->memory[i];
  vm->memory[i] = vm->memory[j];
  vm->memory[j] = a;
  return SUCCESS;
}

int16_t sign_extend(uint16_t n, unsigned int num_bits)
{
  if ((n >> (num_bits - 1)) & 1)
    n |= (0xFFFF << num_bits);
  return n;
}

int one_arg_call(struct StackVM *vm, int16_t (*f)(int16_t))
{
  int16_t a;
  int16_t value;
  RETURN_CHECK(pop(vm, &a));
  RETURN_CHECK(push(vm, f(a)));
  return SUCCESS;
}

int two_arg_call(struct StackVM *vm, int16_t (*f)(int16_t, int16_t))
{
  struct Pair *p;
  RETURN_CHECK(pop2(vm, p));
  RETURN_CHECK(push(vm, f(p->a, p->b)));
  return SUCCESS;
}

int two_arg_peek_call(struct StackVM *vm, int16_t (*f)(int16_t, int16_t))
{
  struct Pair *p;
  RETURN_CHECK(peek2(vm, p));
  RETURN_CHECK(push(vm, f(p->a, p->b)));
  return SUCCESS;
}

int two_arg_peek_comp(struct StackVM *vm, int16_t (*f)(int16_t, int16_t))
{
  struct Pair *p;
  RETURN_CHECK(peek2(vm, p));
  vm->c_flag = f(p->a, p->b);
  return SUCCESS;
}

int handle_instruction(struct StackVM *vm, uint16_t instruction)
{
  unsigned int operation = instruction >> 13;
  unsigned int operand = instruction & ((1 << 13) - 1);
  switch (operation)
  {
  // Push an immediate value on the stack
  case OP_PUSH:
  {
    int16_t imm13 = sign_extend(operand, 13);
    return push(vm, imm13);
  }

  // Load a value from memory onto the top of the stack
  case OP_LOAD:
  {
    uint16_t addr13 = operand;
    int16_t value = vm->memory[addr13];
    return push(vm, value);
  }

  // Pop the top value off of the stack and store in memory
  case OP_STOR:
  {
    uint16_t addr13 = operand;
    int16_t value;
    RETURN_CHECK(pop(vm, &value));
    vm->memory[addr13] = value;
    return SUCCESS;
  }

  // Moves the instruction pointer to the address specified by the immediate value
  case OP_JMP:
  {
    uint16_t addr13 = operand;
    vm->reg[IP] = addr13;
    return SUCCESS;
  }

  // If the conditional flag is set to True, move the instruction pointer to the address specified by the immediate value
  case OP_BR:
  {
    uint16_t addr13 = operand;
    if (vm->c_flag == TRUE)
      vm->reg[IP] = addr13;
    return SUCCESS;
  }

  // Swaps the current TOS with the value n addresses above, where n is a 13-bit immediate
  case OP_SWAP:
  {
    uint16_t imm13 = operand;
    return swap(vm, imm13);
  }

  // Executes a trap code
  case OP_TRAP:
  {
    switch (operand)
    {
    case TRAP_GETC:
    {
      char input = (char)getchar();
      RETURN_CHECK(push(vm, input));
    }

    case TRAP_PUTC:
    {
      int16_t output;
      RETURN_CHECK(pop(vm, &output));
      output &= 0x00FF; // Zero out the top 8 bits
      putchar((char)output);
    }

    case TRAP_HALT:
    {
      return EXCEPTION;
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
      RETURN_CHECK(pop(vm, &a));
      return SUCCESS;
    }
    case NO_ADD:
    {
      return two_arg_call(vm, f_add);
    }
    case NO_SUB:
    {
      return two_arg_call(vm, f_subtract);
    }
    case NO_MULT:
    {
      return two_arg_call(vm, f_multiply);
    }
    case NO_MULTC:
    {
      struct Pair *p;
      RETURN_CHECK(pop2(vm, p));
      int32_t result = p->a * p->b;
      int16_t hi_result = (int16_t)hi((uint32_t)result);
      int16_t lo_result = (int16_t)lo((uint32_t)result);
      RETURN_CHECK(push(vm, hi_result));
      RETURN_CHECK(push(vm, lo_result));
      return SUCCESS;
    }
    case NO_DIV:
    {
      struct Pair *p;
      RETURN_CHECK(pop(vm, p));
      if (p->b == 0)
        return DIVISION_BY_ZERO_ERROR;
      RETURN_CHECK(push(vm, p->a / p->b));
      return SUCCESS;
    }
    case NO_MOD:
    {
      struct Pair *p;
      RETURN_CHECK(pop(vm, p));
      if (p->b == 0)
        return DIVISION_BY_ZERO_ERROR;
      RETURN_CHECK(push(vm, p->a % p->b));
      return SUCCESS;
    }
    case NO_AND:
    {
      return two_arg_call(vm, f_and);
    }
    case NO_OR:
    {
      return two_arg_call(vm, f_or);
    }
    case NO_NOT:
    {
      return one_arg_call(vm, f_not);
    }
    case NO_SHFTL:
    {
      return two_arg_call(vm, f_left_shift);
    }
    case NO_SHFTR:
    {
      return two_arg_call(vm, f_right_shift);
    }
    case NO_EQ:
    {
      return two_arg_peek_comp(vm, f_equals);
    }
    case NO_LT:
    {
      return two_arg_peek_comp(vm, f_less_than);
    }
    case NO_GT:
    {
      return two_arg_peek_comp(vm, f_greater_than);
    }
    case NO_LEQ:
    {
      return two_arg_peek_comp(vm, f_leq);
    }
    case NO_GEQ:
    {
      return two_arg_peek_comp(vm, f_geq);
    }
    case NO_DUP:
    {
      int16_t a;
      RETURN_CHECK(peek(vm, &a));
      RETURN_CHECK(push(vm, a));
      return SUCCESS;
    }
    case NO_SWAPS:
    {
      uint16_t n;
      RETURN_CHECK(pop(vm, &n));
      RETURN_CHECK(swap(vm, n));
      return SUCCESS;
    }
    case NO_LOADS:
    {
      uint16_t address;
      RETURN_CHECK(pop(vm, &address));
      int16_t value = vm->memory[address];
      return push(vm, value);
    }
    case NO_LOADSI:
    {
      uint16_t address;
      RETURN_CHECK(pop(vm, &address));
      int16_t value = vm->memory[vm->memory[address]];
      return push(vm, value);
    }
    case NO_STORS:
    {
      struct Pair *p;
      RETURN_CHECK(pop2(vm, p));
      int16_t address = p->a;
      int16_t value = p->b;
      vm->memory[address] = value;
      return SUCCESS;
    }
    case NO_STORSI:
    {
      struct Pair *p;
      RETURN_CHECK(pop2(vm, p));
      int16_t address = p->a;
      int16_t value = p->b;
      vm->memory[vm->memory[address]] = value;
      return SUCCESS;
    }
    case NO_JUMPS:
    {
      uint16_t address;
      RETURN_CHECK(pop(vm, &address));
      vm->reg[IP] = address;
      return SUCCESS;
    }
    case NO_BRS:
    {
      uint16_t address;
      RETURN_CHECK(pop(vm, &address));
      if (vm->c_flag == TRUE)
        vm->reg[IP] = address;
      return SUCCESS;
    }
    case NO_RET:
    {
      return RETURN;
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

  // Initializing VM
  struct StackVM *vm;
  init_stack_vm(vm);

  // Fetching instructions
  unsigned int file_length = program_length(file) / 2;
  printf("%d\n", file_length);
  fetch_instructions(file, vm, file_length);
  fclose(file);

  // Main loop
  uint16_t instruction;
  while (1)
  {
    instruction = vm->memory[vm->reg[IP]++];
    RETURN_CHECK(handle_instruction(vm, instruction));
  }
}