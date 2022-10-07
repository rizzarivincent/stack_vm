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

#endif