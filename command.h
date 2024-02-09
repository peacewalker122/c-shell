#pragma once

#include "parser.h"
typedef struct {
  char ***command; // [][]char
  int avaible_arguments;
} command;

typedef struct {
  command **commands;
  int commands_len;
  /* int offset; */
  char *write_dst;
  char *read_src;
  char *error_dst;
  int is_background;
} master_command;

// add a command into commands struct
void add_command(master_command *cmds, command *cmd);
master_command *initialize_command();
master_command *new_commands(parser *p);
command *parse_command(parser *p);
