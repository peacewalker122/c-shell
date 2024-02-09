#include "lex.h"

lexer *new_lexer(char *input) {
  lexer *l = (lexer *)malloc(sizeof(lexer));
  l->input = input;
  l->ch = input[0];

  return l;
}

void read_char(lexer *l) {
  if (l->read_position >= strlen(l->input)) {
    l->ch = 0;
  } else {
    l->ch = l->input[l->read_position];
  }
  l->position = l->read_position;
  l->read_position += 1;
}

size_t is_letter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '.');
}

void skip_whitespace(lexer *l) {
  while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
    read_char(l);
  }
}

char *read_identifier(lexer *l) {
  size_t position = l->position;

  while (is_letter(l->ch)) {
    read_char(l);
  }

  char *res = (char *)malloc(l->position - position + 1);

  strncpy(res, l->input + position, l->position - position);

  return res;
}

char *char_identifier(lexer *l, char c) {
  size_t position = l->position;

  while (l->ch == c) {
    read_char(l);
  }

  char *res = (char *)malloc(l->position - position + 1);
  strncpy(res, l->input + position, l->position - position);

  return res;
}

token_kind token_type(char *ident) { return ARGUMENT; }

token *next_token(lexer *l) {
  token *res = (token *)malloc(sizeof(token));

  skip_whitespace(l);

  switch (l->ch) {
  case '\0':
    res->type = ENDOFLINE;
    res->literal = (char *)"\0";
    break;
  case '>':
    if (l->ch == '>') {
      res->literal = char_identifier(l, '>');
      res->type = GREATGREAT;
      return res;
    }

    res->type = GREATER;
    res->literal = (char *)">";
    break;
  case '<':
    res->type = LESSER;
    res->literal = (char *)"<";
    break;
  case '&':
    res->type = AMPERSAND;
    res->literal = (char *)"&";
    break;
  case '|':
    res->type = PIPE;
    res->literal = (char *)"|";
    break;
  case ';':
    res->type = SEMICOLON;
    res->literal = (char *)";";
    break;
  case '-':
    res->type = HYPEN;
    res->literal = (char *)"-";
    break;
  default:
    if (is_letter(l->ch)) {
      res->literal = read_identifier(l);
      res->type = token_type(res->literal);
      return res;
    } else {
      res->type = ILLEGALTOKEN;
      res->literal = (char *)"";
      break;
    }
  }

  read_char(l);
  return res;
}
