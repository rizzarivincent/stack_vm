#include "assembler.h"

int name_index(const char **arr, const char *s)
{
  for (int i = 0; i < sizeof(arr) / sizeof(char *); i++)
  {
    if (strcicmp(arr[i], s) == 0)
      return i;
  }
  return -1;
}

int load_asm_file(FILE *file, const char *file_name)
{
  if ((file = fopen(file_name, "r")) == NULL)
  {
    fclose(file);
    printf("File: %s not found.\n", file_name);
    return 1;
  }
}

int get_token_count(const FILE *file)
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

int get_tokens(const FILE *file, struct Token *tokens, int token_count)
{
  char buffer[BUFFER_SIZE];
  int ip = 0;
  for (int i = 0; i < token_count; i++)
  {
    fscanf(file, "%127s ", buffer);
    if (get_token_from_string(buffer, tokens + i, INVALID_TOKEN, ip) != 0)
      return 1;
    // Take in next string as an imm, addr, jump label, or var label
    if (tokens[i].type == OP_OPERATION_TOKEN)
    {
      i++;
      if (get_token_from_string(buffer, tokens + i, OP_OPERATION_TOKEN, ip) != 0)
        return 1;
      ip++;
    }
    // Increment instruction pointer if needed
    if (tokens[i].type == NO_OP_OPERATION_TOKEN || tokens[i].type == TRAP_OPERATION_TOKEN)
      ip++;
  }
  return 0;
}

int get_token_from_string(const char *s, struct Token *t, const int prev_type, int ip)
{
  int idx;
  // If prev line was operand operation, then this *should be* an operand type (hex, decimal, jump label, or var label)
  if (prev_type == OP_OPERATION_TOKEN)
  {
    if (is_hex_num(s))
    {
      t->type = OPERAND_TOKEN;
      t->value = (uint16_t)get_hex(s);
      return 0;
    }
    else if (is_dec_num(s))
    {
      t->type = OPERAND_TOKEN;
      t->value = (uint16_t)get_dec(s);
      return 0;
    }
    else if (is_valid_label_name(s))
    {
      t->type = LABEL_USE_TOKEN;
      t->name = malloc(sizeof(char) * strlen(s));
      strcpy(t->name, s);
      return 0;
    }
    else
    {
      printf("Invalid operand token.\n");
      return 1;
    }
    t->ip = ip;
    return 0;
  }

  // TODO: Check for keyword
  // TEMP: Just checking for global keyword
  if (strcicmp(s, "entry") == 0)
  {
    t->type = KEYWORD_TOKEN;
    t->name = malloc(sizeof(char) * strlen("entry"));
    strcpy(t->name, "entry");
  }
  // Check for operand operation
  else if ((idx = name_index(op_names, s)) != -1)
  {
    t->type = OP_OPERATION_TOKEN;
    t->value = idx;
  }
  // Check for no operand operation
  else if ((idx = name_index(no_names, s)) != -1)
  {
    t->type = NO_OP_OPERATION_TOKEN;
    t->value = idx;
  }
  // Check for trap operation
  else if ((idx = name_index(trap_names, s)) != -1)
  {
    t->type = TRAP_OPERATION_TOKEN;
    t->value = idx;
  }
  // Check for jump label
  else if (is_jump_label(s))
  {
    if (!is_valid_label_name(s))
    {
      printf("Invalid jump label name.\n");
      return 1;
    }
    t->type = JUMP_LABEL_TOKEN;
    t->name = malloc(sizeof(char) * (strlen(s) - 1));
    strncpy(t->name, s, (strlen(s) - 1));
  }
  // TODO: Check for var label
  else
  {
    return 1;
  }
  t->ip = ip;
  return 0;
}

int main(int argc, char *argv[])
{
  // Checking arg count
  if (argc != 2)
  {
    printf("Usage: ./assembler [file]\n");
    return 1;
  }

  // Loading file
  FILE *file;
  if (load_asm_file(file, argv[1]) == 1)
    return 1;

  // Get number of tokens and all the tokens
  int num_tokens = get_token_count(file);
  struct Token *tokens = malloc(sizeof(struct Token) * num_tokens);
  if (get_tokens(file, tokens, num_tokens) != 0)
    return 1;
  int num_instructions = tokens[num_tokens - 1].ip;

  // Close the file
  fclose(file);

  // Getting number of jump labels and creating jump table
  struct TableEntry *jump_table = malloc(sizeof(struct TableEntry) * JUMP_TABLE_MAX_SIZE);
  int jump_count = 0;
  for (int i = 0; i < num_tokens; i++)
    if (tokens[i].type == JUMP_LABEL_TOKEN)
    {
      jump_table[jump_count].name = malloc(sizeof(char) * len(tokens[i].name));
      strcpy(jump_table[jump_count].name, tokens[i].name);
      jump_table[jump_count].address = tokens[i].ip;
      jump_count++;
    }

  // Creating instructions
  uint16_t *instructions = malloc(sizeof(uint16_t) * num_instructions);
  int ip = 0;
  for (int i = 0; i < num_tokens; i++)
  {
  }

  // Placing instructions into VM memory

  // Freeing allocated memory
  for (int i = 0; i < num_tokens; i++)
    if (tokens[i].type == LABEL_USE_TOKEN ||
        tokens[i].type == KEYWORD_TOKEN ||
        tokens[i].type == JUMP_LABEL_TOKEN)
      free(tokens[i].name);
  free(tokens);
  for (int i = 0; i < jump_count; i++)
    free(jump_table[i].name);
  free(jump_table);
  free(instructions);
  return 0;
}