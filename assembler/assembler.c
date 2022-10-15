#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../stack_vm/stack_vm.h"

#define BUFFER_SIZE 128
#define JUMP_TABLE_MAX_SIZE 256

struct JumpTableEntry
{
  char *name;
  uint16_t address;
};

int fill_jump_table(char **tokens, int num_tokens, struct JumpTableEntry *jump_table)
{
  char *global;
  int num_global = 0;

  for (int i = 0; i < num_tokens; i++)
  {
    // Checking for jump
    if (tokens[i][strlen(tokens[i]) - 1] == ':')
    {
    }
    // Checking for global string
    if (strcicmp(tokens[i], "global") == 0)
    {
      global = tokens[++i];
      num_global++;
    }
  }

  if (num_global != 1)
  {
    printf("Failed to find the global entry point.\n");
    return 1;
  }
}

int load_asm_file(FILE *file, char *file_name)
{
  if ((file = fopen(file_name, "r")) == NULL)
  {
    fclose(file);
    printf("File: %s not found.\n", file_name);
    return 1;
  }
}

int get_num_tokens(FILE *file)
{
  char buffer[BUFFER_SIZE];
  int num_tokens = 0;
  while (fscanf(file, "%127s ", buffer) == 1)
  {
    num_tokens++;
  }
  fseek(file, 0, SEEK_SET);
  return num_tokens;
}

void get_tokens(FILE *file, char **tokens, int num_tokens)
{
  char buffer[BUFFER_SIZE];
  for (int i = 0; i < num_tokens; i++)
  {
    fscanf(file, "%127s ", buffer);
    tokens[i] = malloc(strlen(buffer));
    strcpy(tokens[i], buffer);
  }
  return;
}

int name_index(char **arr, char *s)
{
  for (int i = 0; i < sizeof(arr) / sizeof(char *); i++)
  {
    if (strcicmp(arr[i], s) == 0)
      return i;
  }
  return -1;
}

int main(int argc, char *argv[])
{
  // Getting tokens from file
  if (argc != 2)
  {
    printf("Usage: ./assembler [file]\n");
    return 1;
  }
  FILE *file;
  if (load_asm_file(file, argv[1]) == 1)
    return 1;
  int num_tokens = get_num_tokens(file);
  char **tokens = malloc(sizeof(char *) * num_tokens);
  get_tokens(file, tokens, num_tokens);
  fclose(file);

  struct JumpTableEntry *jump_table = malloc(sizeof(struct JumpTableEntry) * JUMP_TABLE_MAX_SIZE);

  // Going through tokens, creating machine instructions
  uint16_t *output_instructions = malloc(sizeof(uint16_t) * num_tokens);
  int instruction_count = 0;
  char *token, *dummy;
  uint16_t operation, operand;
  int op_idx, no_idx, trap_idx;
  for (int i = 0; i < num_tokens; i++)
  {
    token = tokens[i];
    op_idx = name_index(op_names, token);     // Get position in op_names array
    no_idx = name_index(no_names, token);     // Get position in no_names array
    trap_idx = name_index(trap_names, token); // Get position in trap_names array

    if (op_idx != -1) // Normal operand operation
    {
      operation = op_idx;
      operand = (uint16_t)strtoul(tokens[++i], dummy, 10);
    }
    else if (no_idx != -1) // No-operand operation
    {
      operation = OP_NO;
      operand = no_idx;
    }
    else if (trap_idx != -1) // Trap operation
    {
      operation = OP_TRAP;
      operand = trap_idx;
    }
    else // Not found in any array: invalid
    {
      printf("Invalid instruction.\n");
      return 1;
    }

    // Move op code to first 3 bits, make sure operand is max 13 bits long, combine to get instruction, add to instruction output
    output_instructions[instruction_count++] = (operation << 13) | (operand & 0x1FFF);
  }
  // Reallocating the size of instructions
  output_instructions = realloc(output_instructions, sizeof(uint16_t) * instruction_count);

  // Freeing allocated memory
  for (int i = 0; i < num_tokens; i++)
  {
    free(tokens[i]);
  }
  free(tokens);
  free(jump_table);
  free(output_instructions);
  return 0;
}