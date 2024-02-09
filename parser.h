#pragma once

#include "lex.h"

typedef struct {
  lexer *l;

  token cur_token;
  token peek_token; // contain the next token from the current token;
} parser;

void get_token(parser *p);
parser *new_parser(lexer *l);
