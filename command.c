#include "command.h"
#include "lex.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>
#define COMMAND_LENGTH_THRESHOLD 10
#define COMMANDS_LENGTH 10

master_command *initialize_command() {
  master_command *cmds = (master_command *)malloc(sizeof(master_command));
  cmds->commands =
      (command **)malloc(sizeof(command *) * (COMMAND_LENGTH_THRESHOLD));
  cmds->commands_len = 0;

  return cmds;
}

void add_command(master_command *cmds, command *cmd) {
  cmds->commands[cmds->commands_len] = cmd;
  cmds->commands_len++;

  if (cmds->commands_len >= COMMAND_LENGTH_THRESHOLD) {
    cmds->commands_len += COMMAND_LENGTH_THRESHOLD;

    cmds->commands = (command **)realloc(cmds->commands, cmds->commands_len);
  }
}

master_command *new_commands(parser *p) {
  master_command *c = initialize_command();

  command *cmd = parse_command(p);
  add_command(c, cmd);

  return c;
}

command *parse_command(parser *p) {
  command *res = (command *)malloc(sizeof(command));

  // NOTE: initialize the array of 'string'
  char ***val = (char ***)malloc(sizeof(char **) * COMMANDS_LENGTH);

  int first_index = 0;
  int second_index = 0;

  val[first_index] = (char **)malloc(sizeof(char *) * COMMANDS_LENGTH);
  while (1) {
    if (p->cur_token.type == ENDOFLINE) {
      break;
    }

    // NOTE: implement to retrieve it by get the next token.
    // if the next token were END_TOKEN stop the process
    // NOTE: Handle process such like &&, | and ;
    switch (p->cur_token.type) {
    case TOKEN_END:
    case ENDOFLINE:
      p->cur_token.type = ENDOFLINE;

      break;
    case SEMICOLON:
    case PIPE:
    case AMPERSAND:
      first_index++;
      second_index = 0;
      val[first_index] = (char **)malloc(sizeof(char *) * COMMANDS_LENGTH);

      get_token(p);
      continue;
    default:
      val[first_index][second_index] = p->cur_token.literal;
      second_index++;

      get_token(p);

      // WARN: consider to use offset, to avoid unecessary reallocation
      if (second_index >= COMMANDS_LENGTH) {
        val[second_index] =
            (char **)realloc(val[second_index], second_index + COMMANDS_LENGTH);
      }
    }
  }

  res->command = val;

  return res;
}
