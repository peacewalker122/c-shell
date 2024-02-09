#include "parser.h"

void get_token(parser *p) {
  p->cur_token = p->peek_token;
  p->peek_token = *next_token(p->l);
}

parser *new_parser(lexer *l) {
  parser *p = (parser *)malloc(sizeof(parser));

  p->l = l;
  get_token(p);
  get_token(p);

  return p;
}
