#include "stack_vm_test.h"

int push_one_value()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 13), SUCCESS);
  ASSERT_EQ(vm.reg[SP], STACK_START - 1);
  ASSERT_EQ(vm.memory[vm.reg[SP] + 1], 13);
  AFTER();
}

int push_two_values()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 5), SUCCESS);
  ASSERT_EQ(vm.reg[SP], STACK_START - 1);
  ASSERT_EQ(push(&vm, 7), SUCCESS);
  ASSERT_EQ(vm.reg[SP], STACK_START - 2);
  ASSERT_EQ(vm.memory[vm.reg[SP] + 1], 7);
  ASSERT_EQ(vm.memory[vm.reg[SP] + 2], 5);
  AFTER();
}

int push_and_pop_one_value()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 19), SUCCESS);
  int16_t n;
  ASSERT_EQ(pop(&vm, &n), SUCCESS);
  ASSERT_EQ(n, 19);
  ASSERT_EQ(vm.reg[SP], STACK_START);
  AFTER();
}

int push_and_pop_two_values()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 12), SUCCESS);
  ASSERT_EQ(push(&vm, 115), SUCCESS);
  int16_t n1, n2;
  ASSERT_EQ(pop(&vm, &n2), SUCCESS);
  ASSERT_EQ(pop(&vm, &n1), SUCCESS);
  ASSERT_EQ(n2, 115);
  ASSERT_EQ(n1, 12);
  AFTER();
}

int push_and_peek_one_value()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 56), SUCCESS);
  int16_t n;
  ASSERT_EQ(peek(&vm, &n), SUCCESS);
  ASSERT_EQ(n, 56);
  ASSERT_EQ(vm.reg[SP], STACK_START - 1);
  AFTER();
}

int push_and_peek_two_values()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 41), SUCCESS);
  int16_t n1;
  ASSERT_EQ(peek(&vm, &n1), SUCCESS);
  ASSERT_EQ(push(&vm, 52), SUCCESS);
  int16_t n2;
  ASSERT_EQ(peek(&vm, &n2), SUCCESS);
  ASSERT_EQ(n1, 41);
  ASSERT_EQ(n2, 52);
  ASSERT_EQ(vm.reg[SP], STACK_START - 2);
  AFTER();
}

int push_two_values_pop2()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 20), SUCCESS);
  ASSERT_EQ(push(&vm, 42), SUCCESS);
  struct Pair p;
  ASSERT_EQ(pop2(&vm, &p), SUCCESS);
  ASSERT_EQ(p.a, 42);
  ASSERT_EQ(p.b, 20);
  ASSERT_EQ(vm.reg[SP], STACK_START);
  AFTER();
}

int push_two_values_peek2()
{
  BEFORE();
  ASSERT_EQ(push(&vm, 15), SUCCESS);
  ASSERT_EQ(push(&vm, 428), SUCCESS);
  struct Pair p;
  ASSERT_EQ(peek2(&vm, &p), SUCCESS);
  ASSERT_EQ(p.a, 428);
  ASSERT_EQ(p.b, 15);
  ASSERT_EQ(vm.reg[SP], STACK_START - 2);
  AFTER();
}

int push_many_values()
{
  BEFORE();
  for (int i = 0xFFFF; i >= 0x8000; i--)
  {
    ASSERT_EQ(push(&vm, i), SUCCESS);
  }
  AFTER();
}

int push_and_pop_many_values()
{
  BEFORE();
  for (int i = 0xFFFF; i >= 0x8000; i--)
  {
    ASSERT_EQ(push(&vm, i), SUCCESS);
  }
  int16_t n;
  for (int i = 0x8000; i >= 0xFFFF; i++)
  {
    ASSERT_EQ(pop(&vm, &n), SUCCESS);
    ASSERT_EQ(n, i);
  }
  AFTER();
}

int push_two_and_swap()
{
  BEFORE();
  push(&vm, 2);
  push(&vm, 90);
  ASSERT_EQ(swap(&vm, 1), SUCCESS);
  int16_t n1, n2;
  ASSERT_EQ(pop(&vm, &n1), SUCCESS);
  ASSERT_EQ(pop(&vm, &n2), SUCCESS);
  ASSERT_EQ(n1, 2);
  ASSERT_EQ(n2, 90);
  AFTER();
}

int push_three_and_swap()
{
  BEFORE();
  push(&vm, 10);
  push(&vm, 20);
  push(&vm, 30);
  ASSERT_EQ(swap(&vm, 2), SUCCESS);
  int16_t n1, n2, n3;
  ASSERT_EQ(pop(&vm, &n1), SUCCESS);
  ASSERT_EQ(pop(&vm, &n2), SUCCESS);
  ASSERT_EQ(pop(&vm, &n3), SUCCESS);
  ASSERT_EQ(n1, 10);
  ASSERT_EQ(n2, 20);
  ASSERT_EQ(n3, 30);
  AFTER();
}

int push_too_many_values_exception()
{
  BEFORE();
  for (int i = 0xFFFF; i >= 0x8000; i--)
  {
    push(&vm, i);
  }
  ASSERT_EQ(push(&vm, 0), FULL_STACK_ERROR);
  AFTER();
}

int pop_empty_stack_exception()
{
  BEFORE();
  int16_t n;
  ASSERT_EQ(pop(&vm, &n), EMPTY_STACK_ERROR);
  AFTER();
}

int pop2_empty_stack_exception()
{
  BEFORE();
  struct Pair p;
  ASSERT_EQ(pop2(&vm, &p), EMPTY_STACK_ERROR);
  AFTER();
}

int peek_empty_stack_exception()
{
  BEFORE();
  int16_t n;
  ASSERT_EQ(peek(&vm, &n), EMPTY_STACK_ERROR);
  AFTER();
}

int peek2_empty_stack_exception()
{
  BEFORE();
  struct Pair p;
  ASSERT_EQ(peek2(&vm, &p), EMPTY_STACK_ERROR);
  AFTER();
}

int push_pop2_empty_stack_exception()
{
  BEFORE();
  push(&vm, 5);
  struct Pair p;
  ASSERT_EQ(pop2(&vm, &p), EMPTY_STACK_ERROR);
  AFTER();
}

int push_peek2_empty_stack_exception()
{
  BEFORE();
  push(&vm, 5);
  struct Pair p;
  ASSERT_EQ(peek2(&vm, &p), EMPTY_STACK_ERROR);
  AFTER();
}

int push_pop_twice_empty_stack_exception()
{
  BEFORE();
  push(&vm, 6);
  int16_t n;
  pop(&vm, &n);
  ASSERT_EQ(pop(&vm, &n), EMPTY_STACK_ERROR);
}

int main()
{
  push_one_value();
  push_two_values();
  push_and_pop_one_value();
  push_and_pop_two_values();
  push_and_peek_one_value();
  push_and_peek_two_values();
  push_two_values_pop2();
  push_two_values_peek2();
  push_many_values();
  push_and_pop_many_values();
  push_two_and_swap();
  push_three_and_swap();
  push_too_many_values_exception();
  pop_empty_stack_exception();
  pop2_empty_stack_exception();
  peek_empty_stack_exception();
  peek2_empty_stack_exception();
  push_pop2_empty_stack_exception();
  push_peek2_empty_stack_exception();
  push_pop_twice_empty_stack_exception();
  return 0;
}