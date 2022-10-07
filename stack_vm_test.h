#include "stack_vm.h"

#ifndef STACK_VM_TEST_H
#define STACK_VM_TEST_H

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

enum assert_codes
{
  PASS = 0,
  FAIL
};

#define BEFORE()          \
  struct StackVM vm;      \
  init_stack_vm(&vm);     \
  int assert_counter = 0; \
  printf("Beginning test: %s\n", __func__);

#define AFTER()                                               \
  printf(ANSI_COLOR_GREEN "Test passed!\n" ANSI_COLOR_RESET); \
  return PASS;

#define ASSERT_EQ(a, b)                                                  \
  assert_counter++;                                                      \
  if ((a) != (b))                                                        \
  {                                                                      \
    printf("Failed assert number %d: %d != %d\n", assert_counter, a, b); \
    return FAIL;                                                         \
  }

// Testing push, pop, peek, swap normal functions
int push_one_value();
int push_two_values();
int push_and_pop_one_value();
int push_and_pop_two_values();
int push_and_peek_one_value();
int push_and_peek_two_values();
int push_two_values_pop2();
int push_two_values_peek2();
int push_many_values();
int push_and_pop_many_values();
int push_two_and_swap();
int push_three_and_swap();
// Testing push, pop, peek, swap exceptions
int push_too_many_values_exception();
int pop_empty_stack_exception();
int pop2_empty_stack_exception();
int peek_empty_stack_exception();
int peek2_empty_stack_exception();
int push_pop2_empty_stack_exception();
int push_peek2_empty_stack_exception();
int push_pop_twice_empty_stack_exception();
int swap_empty_stack_exception();
int push_swap_empty_stack_exception();
int swap_stack_too_small_exception();
int push_swap_stack_too_small_exception();
// Testing PUSH
int PUSH_one();
int PUSH_two();
int PUSH_many_values();
int PUSH_many_values_exception();
// Testing LOAD
int LOAD_one();
int LOAD_many_values_exception();
// Testing STORE
int STORE_one();
int STORE_two();
int STORE_twice_same_address();
// Testing SWAP
int SWAP_one();
int SWAP_two();
int SWAP_stack_too_small_exception();
// Testing JMP
int JMP_once();
int JMP_twice();
// Testing BR
int BR_true_jump();
int BR_false_no_jump();
// Testing ADD
int push_two_ADD();
int push_three_ADD_twice();
int ADD_two_large_pos_values();
int ADD_two_large_neg_values();
// Testing SUB
int push_two_SUB();
int push_three_SUB_twice();
int SUB_neg_from_pos();
int SUB_pos_from_neg();
int SUB_pos_from_pos_pos_result();
int SUB_pos_from_pos_neg_result();
int SUB_neg_from_neg_pos_result();
int SUB_neg_from_neg_neg_result();
// Testing MULT
int push_two_MULT();
int push_three_MULT_twice();
int MULT_pos_by_pos();
int MULT_pos_by_neg();
int MULT_neg_by_pos();
int MULT_neg_by_neg();
int MULT_two_large_values();
// Testing MULTC

// Testing DIV

// Testing MOD

#endif