#include "stack_vm.h"

#ifndef STACK_VM_TEST_H
#define STACK_VM_TEST_H

#define ASSERT_EQ(a, b) \
  if (a != b)           \
    return 1;

#define BEFORE()      \
  struct StackVM *vm; \
  init_stack_vm(vm);

// Testing push, pop, peek normal functions
int push_one_value();
int push_two_values();
int push_and_pop_one_value();
int push_and_pop_two_values();
int push_and_peek_one_value();
int push_and_peek_two_values();
int push_two_values_pop2();
int push_two_values_peek2();
// Testing push, pop, peek exceptions
int push_too_many_values_exception();
int pop_empty_stack_exception();
int pop2_empty_stack_exception();
int peek_empty_stack_exception();
int peek2_empty_stack_exception();
int push_pop2_empty_stack_exception();
int push_peek2_empty_stack_exception();
// Testing swap and exceptions

#endif