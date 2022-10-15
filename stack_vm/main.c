#include <stdlib.h>
#include "stack_vm.h"

unsigned int program_length(FILE *file)
{
  fseek(file, 0, SEEK_END);
  unsigned int byte_length = (unsigned int)ftell(file);
  fseek(file, 0, SEEK_SET);
  return byte_length;
}

int main(int argc, char *argv[])
{
  // Making sure file was supplied
  char *file_name = argv[1];
  if (argc != 2)
  {
    printf("Usage: ./main [file]\n");
    return 1;
  }
  FILE *file;
  if ((file = fopen(file_name, "rb")) == NULL)
  {
    printf("File failed to open.\n");
    fclose(file);
    return 1;
  }

  // Initializing VM
  struct StackVM vm;
  init_stack_vm(&vm);

  // Fetching bytes from file
  unsigned int byte_length = program_length(file);
  printf("Byte Length: %d\n", byte_length); // TEMP
  char *byte_array = malloc(sizeof(char) * byte_length);
  for (int i = 0; i < byte_length; i++)
  {
    byte_array[i] = getc(file);
    // printf("%d\n", (int)byte_array[i]);
  }
  fclose(file);

  // Creating instructions from bytes
  unsigned int instruction_length = byte_length / 2;
  uint16_t *instructions = malloc(sizeof(uint16_t) * instruction_length);
  for (int i = 0; i < instruction_length; i++)
  {
    instructions[i] = (byte_array[2 * i] << 8) + byte_array[2 * i + 1];
  }

  // Placing instructions into VM memory
  for (int i = 0; i < instruction_length; i++)
  {
    vm.memory[INSTRUCTION_START + i] = instructions[i];
  }
  vm.memory[INSTRUCTION_START + instruction_length] = (uint16_t)0xC008;

  // Running the instructions
  uint16_t instruction;
  while (vm.reg[IP] - INSTRUCTION_START <= instruction_length)
  {
    instruction = vm.memory[vm.reg[IP]++];
    RETURN_CHECK(handle_instruction(&vm, instruction));
  }

  // Freeing allocated variables
  free(byte_array);
  free(instructions);
}