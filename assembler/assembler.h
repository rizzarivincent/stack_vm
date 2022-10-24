#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include "../stack_vm/stack_vm.h"

#define BUFFER_SIZE 128
#define JUMP_TABLE_MAX_SIZE 256

struct TableEntry
{
  char *name;
  uint16_t address;
};

enum TokenType
{
  OP_OPERATION_TOKEN = 0,
  NO_OP_OPERATION_TOKEN,
  TRAP_OPERATION_TOKEN,
  JUMP_LABEL_TOKEN,
  VAR_LABLE_TOKEN,
  OPERAND_TOKEN,
  LABEL_USE_TOKEN,
  KEYWORD_TOKEN,
  INVALID_TOKEN
};

struct Token
{
  int type;
  char *name;     // Used if jump label (use), var label (use)
  uint16_t value; // Used if operand
  uint16_t ip;    // IP associated with this token
};

int name_index(const char **arr, const char *s);
int load_asm_file(FILE *file, const char *file_name);
int file_exists(char *filename);
int output_to_file(uint16_t *instructions, int num_instructions, char *filename);
int get_token_count(const FILE *file);
int get_tokens(const FILE *file, struct Token *tokens, int token_count);
int get_token_from_string(const char *s, struct Token *t, const int prev_type, int ip);
int find_from_table(const struct TableEntry *table, const int table_size, const char *s);

int is_hex(const char *s);
int is_dec(const char *s);
int is_jump_label(const char *s);
int is_valid_label_name(const char *s);

int get_hex(const char *s);
int get_dec(const char *s);

int get_int_from_hex_digit(const char c);

#endif