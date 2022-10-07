#ifndef STACK_VM_H
#define STACK_VM_H

#include <stdio.h>
#include <stdint.h>

#define MEMORY_MAX (1 << 16)
#define INSTRUCTION_START (0x3000)
#define STACK_START (0xFFFF)
#define STACK_END (0x8000)

#define FALSE (0)
#define TRUE (1)

#define RETURN_CHECK(FUNCTION_CALL)                \
  {                                                \
    int return_value;                              \
    if ((return_value = FUNCTION_CALL) != SUCCESS) \
      return return_value;                         \
  }

struct StackVM
{
  int16_t memory[MEMORY_MAX];
  uint16_t reg[2];
  int c_flag;
};

struct StackVM init_stack_vm(struct StackVM *vm);

struct Pair
{
  int16_t a;
  int16_t b;
};

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
  NO_EQ,
  NO_LT,
  NO_GT,
  NO_LEQ,
  NO_GEQ,
  NO_DUP,
  NO_SWAPS,
  NO_IP,
  NO_LOADS,
  NO_LOADSI,
  NO_STORS,
  NO_STORSI,
  NO_JUMPS,
  NO_BRS,
  NO_RET
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
  SP
};

enum return_codes
{
  SUCCESS = 0,
  FULL_STACK_ERROR,
  EMPTY_STACK_ERROR,
  STACK_TOO_SMALL_ERROR,
  INVALID_ARGUMENT_ERROR,
  DIVISION_BY_ZERO_ERROR,
  INVALID_INSTRUCTION_ERROR,
  EXCEPTION,
  RETURN
};

// Needed for one and two stack args function calls
static inline int16_t f_add(int16_t a, int16_t b) { return a + b; }
static inline int16_t f_subtract(int16_t a, int16_t b) { return a - b; }
static inline int16_t f_multiply(int16_t a, int16_t b) { return (int16_t)a * b; }
static inline int16_t f_divide(int16_t a, int16_t b) { return a / b; }
static inline int16_t f_mod(int16_t a, int16_t b) { return a % b; }
static inline int16_t f_and(int16_t a, int16_t b) { return a & b; }
static inline int16_t f_or(int16_t a, int16_t b) { return a | b; }
static inline int16_t f_not(int16_t a) { return ~a; }
static inline int16_t f_left_shift(int16_t a, int16_t b) { return (a << b); }
static inline int16_t f_right_shift(int16_t a, int16_t b) { return a >> b; }
static inline int16_t f_equals(int16_t a, int16_t b) { return a == b; }
static inline int16_t f_greater_than(int16_t a, int16_t b) { return a > b; }
static inline int16_t f_less_than(int16_t a, int16_t b) { return a < b; }
static inline int16_t f_geq(int16_t a, int16_t b) { return a >= b; }
static inline int16_t f_leq(int16_t a, int16_t b) { return a <= b; }

// Helper functions to grab lo and hi bits of 32-bit integer
static inline uint16_t lo(uint32_t n) { return n & (0xFFFF); }
static inline uint16_t hi(uint32_t n) { return (n >> 16) & (0xFFFF); }

// Helper to sign extend a number from num_bits bits to 16 bits
int16_t sign_extend(uint16_t n, unsigned int num_bits);

// Functions which directly access the stack
int push(struct StackVM *vm, int16_t n);
int pop(struct StackVM *vm, int16_t *n);
int pop2(struct StackVM *vm, struct Pair *p);
int peek(struct StackVM *vm, int16_t *n);
int peek2(struct StackVM *vm, struct Pair *p);
int swap(struct StackVM *vm, uint16_t n);

// Functions which pop/peek one/two arguments off of the stack and push/change c_flag based on the output of the passed function
int one_arg_call(struct StackVM *vm, int16_t (*f)(int16_t));
int two_arg_call(struct StackVM *vm, int16_t (*f)(int16_t, int16_t));
int two_arg_peek_call(struct StackVM *vm, int16_t (*f)(int16_t, int16_t));
int two_arg_peek_comp(struct StackVM *vm, int16_t (*f)(int16_t, int16_t));

// Functions to grab the file from user input
int handle_instruction(struct StackVM *vm, uint16_t instruction);
unsigned int program_length(FILE *file);
void fetch_instructions(FILE *file, struct StackVM *vm, unsigned int length);

#endif