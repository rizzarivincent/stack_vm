#include "stack_vm_test.h"

int push_one_value()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 13), SUCCESS);
  ASSERT_EQ(vm.memory[vm.reg[SP] + 1], 13);
  AFTER();
}

int push_two_values()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 5), SUCCESS);
  ASSERT_EQ(push(&vm, 7), SUCCESS);
  ASSERT_EQ(vm.memory[vm.reg[SP] + 1], 7);
  ASSERT_EQ(vm.memory[vm.reg[SP] + 2], 5);
  AFTER();
}
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

int main()
{
  push_one_value();
  push_two_values();
  return 0;
}