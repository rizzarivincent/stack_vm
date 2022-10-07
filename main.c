#include "stack_vm.h"

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
  struct StackVM vm;
  init_stack_vm(&vm);

  // Fetching instructions
  unsigned int file_length = program_length(file) / 2;
  printf("%d\n", file_length);
  fetch_instructions(file, &vm, file_length);
  fclose(file);

  // Main loop
  uint16_t instruction;
  while (1)
  {
    instruction = vm.memory[vm.reg[IP]++];
    RETURN_CHECK(handle_instruction(&vm, instruction));
  }
}