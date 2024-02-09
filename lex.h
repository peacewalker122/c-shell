#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TOKEN_END,
  GREATER,        // >
  LESSER,         // <
  GREATGREAT,     // >>
  GREATAMPERSAND, // >&
  PIPE,           // |
  SEMICOLON,      // ;
  HYPEN,          // -
  AMPERSAND,      // &
  ENDOFLINE,      // \0
  ARGUMENT,       // ls, main.c, test.txt
  ILLEGALTOKEN,   // Error
} token_kind;

typedef struct {
  token_kind type;
  char *literal;
} token;

typedef struct {
  char *input;
  size_t position;
  size_t read_position;
  char ch;
} lexer;

// function definition
lexer *new_lexer(char *input);
void read_char(lexer *l);
size_t is_letter(char c);
void skip_whitespace(lexer *l);
char *read_identifier(lexer *l);
char *char_identifier(lexer *l, char c);
token_kind token_type(char *ident);
token *next_token(lexer *l);
