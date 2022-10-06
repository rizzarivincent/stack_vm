#include "stack_vm_test.h"

int push_one_value()
{
  BEFORE();
  int r = push(vm, 1);
  ASSERT_EQ(r, SUCCESS);
}

int push_two_values()
{
}

int push_and_pop_one_value()
{
}

int push_and_pop_two_values()
{
}

int push_and_peek_one_value()
{
}