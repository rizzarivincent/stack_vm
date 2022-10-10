#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stack_vm.h"

#define BUFFER_SIZE 128

// int get_instruction_from_string(char *s, uint16_t *n)
// {
//   if (strcmp(*s, "PUSH") == 0)
//   {
//     *n = OP_PUSH;
//   }
//   else if (strcmp(*s, "LOAD") == 0)
//   {
//     *n = OP_LOAD
//   }
//   else
//   {
//     return 1;
//   }
//   return 0;
// }

// int get_operand_from_string(char *s, uint16_t *n)
// {
// }

// int is_operand_instruction(char *s)
// {
// }

// int is_no_operand_instruction(char *s)
// {
// }

int main(int argc, char *argv[])
{
  // Incorrect number of arguments
  if (argc != 2)
  {
    printf("Usage: ./assembler [file]\n");
    return 1;
  }

  // Loading the file
  char *file_name = argv[1];
  FILE *file;
  if ((file = fopen(file_name, "r")) == NULL)
  {
    fclose(file);
    printf("File: %s not found.\n", file_name);
    return 1;
  }

  // Loop for getting number of tokens
  char buffer[BUFFER_SIZE];
  int token_count = 0;
  while (fscanf(file, "%127s ", buffer) == 1)
  {
    token_count++;
  }

  // Loop for grabbing tokens from input
  fseek(file, 0, SEEK_SET);
  char **tokens = malloc(sizeof(char *) * token_count);
  for (int i = 0; i < token_count; i++)
  {
    fscanf(file, "%127s ", buffer);
    tokens[i] = malloc(strlen(buffer));
    strcpy(tokens[i], buffer);
  }

  // Going through tokens, creating machine instructions
  uint16_t *output_instructions = malloc(sizeof(uint16_t) * token_count);
  int instruction_count = 0;
  uint16_t instruction;
  uint16_t operand;
  for (int i = 0; i < token_count; i++)
  {
    // if (is_operand_instruction(tokens[i]))
    // {
    //   instruction = get_instruction_from_string(tokens[i]);
    //   operand = get_operand_from_string(tokens[++i]);
    //   output_instructions[instruction_count] = create_instruction(instruction, operand);
    // }
    // else if (is_no_operand_instruction(tokens[i]))
    // {
    //   instruction = get_instruction_from_string(tokens[i]);
    //   output_instructions[instruction_count] = instruction;
    // }
    // else
    // {
    //   printf("Invalid instruction error.\n");
    //   return 1;
    // }
    instruction_count++;
  }
  // Reallocating the size of instructions

  // Closing file
  fclose(file);

  // Freeing allocated memory
  for (int i = 0; i < token_count; i++)
  {
    free(tokens[i]);
  }
  free(tokens);
  free(output_instructions);
  return 0;
}