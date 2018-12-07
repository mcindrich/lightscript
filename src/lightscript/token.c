#include <lightscript/token.h>
#include <lightscript/defines.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum ls_token_type_t ls_token_get_char_type(char c) {
  enum ls_token_type_t tt;

  if(c == ';') tt = ls_token_type_semicolon;
  else if(c == ',') tt = ls_token_type_comma;
  else if(c == ':') tt = ls_token_type_colon;
  else if(c == '+') tt = ls_token_type_plus;
  else if(c == '-') tt = ls_token_type_minus;
  else if(c == '*') tt = ls_token_type_multiply;
  else if(c == '/') tt = ls_token_type_divide;
  else if(c == '%') tt = ls_token_type_percent;
  else if(c == '{') tt = ls_token_type_lbrace;
  else if(c == '}') tt = ls_token_type_rbrace;
  else if(c == '(') tt = ls_token_type_lparenth;
  else if(c == ')') tt = ls_token_type_rparenth;
  else if(c == '[') tt = ls_token_type_lbracket;
  else if(c == ']') tt = ls_token_type_rbracket;
  else if(c == '=') tt = ls_token_type_equal;
  else if(c == '$') tt = ls_token_type_dollar;
  else if(c == '@') tt = ls_token_type_at;
  else if(c == '#') tt = ls_token_type_hash;
  else if(c == '<') tt = ls_token_type_less_than;
  else if(c == '>') tt = ls_token_type_greater_than;
  else if(c == '!') tt = ls_token_type_exclamation;
  else if(c == '~') tt = ls_token_type_tilde;
  else if(c == '|') tt = ls_token_type_log_or;
  else if(c == '&') tt = ls_token_type_log_and;
  else tt = ls_token_type_unknown;

  return tt;
}

enum ls_token_type_t ls_token_get_word_type(char *str) {
  enum ls_token_type_t tt;

  if(strcmp(str, "ret") == 0) tt = ls_token_type_return_keyword;
  else if(strcmp(str, "import") == 0) tt = ls_token_type_include_keyword;
  else if(strcmp(str, "def") == 0) tt = ls_token_type_def_keyword;
  else if(strcmp(str, "new") == 0) tt = ls_token_type_new_keyword;
  else if(strcmp(str, "if") == 0) tt = ls_token_type_if_keyword;
  else if(strcmp(str, "elif") == 0) tt = ls_token_type_elsif_keyword;
  else if(strcmp(str, "else") == 0) tt = ls_token_type_else_keyword;
  else if(strcmp(str, "while") == 0) tt = ls_token_type_while_keyword;
  else if(strcmp(str, "for") == 0) tt = ls_token_type_for_keyword;
  else if(strcmp(str, "in") == 0) tt = ls_token_type_in_keyword;
  else if(strcmp(str, "struct") == 0) tt = ls_token_type_object_keyword;
  else if(strcmp(str, "then") == 0) tt = ls_token_type_then_keyword;
  else if(strcmp(str, "do") == 0) tt = ls_token_type_do_keyword;
  else if(strcmp(str, "end") == 0) tt = ls_token_type_end_keyword;
  else if(strcmp(str, "::") == 0) tt = ls_token_type_double_colon;
  else if(strcmp(str, "<=") == 0) tt = ls_token_type_less_equal;
  else if(strcmp(str, ">=") == 0) tt = ls_token_type_greater_equal;
  else if(strcmp(str, "++") == 0) tt = ls_token_type_increment;
  else if(strcmp(str, "--") == 0) tt = ls_token_type_increment;
  else if(strcmp(str, "+=") == 0) tt = ls_token_type_plus_equal;
  else if(strcmp(str, "-=") == 0) tt = ls_token_type_minus_equal;
  else if(strcmp(str, "*=") == 0) tt = ls_token_type_multiply_equal;
  else if(strcmp(str, "/=") == 0) tt = ls_token_type_divide_equal;
  else if(strcmp(str, "==") == 0) tt = ls_token_type_equal_equal;
  else if(strcmp(str, "!=") == 0) tt = ls_token_type_not_equal;
  else if(strcmp(str, "&=") == 0) tt = ls_token_type_and_equal;
  else if(strcmp(str, "|=") == 0) tt = ls_token_type_or_equal;
  else if(strcmp(str, "&&") == 0) tt = ls_token_type_and_op;
  else if(strcmp(str, "||") == 0) tt = ls_token_type_or_op;
  else if(LS_IS_NUMBER(str)) tt = ls_token_type_number;
  else tt = ls_token_type_word;

  return tt;
}

void ls_token_create(struct ls_token_t *tok) {
  tok->type = ls_token_type_unknown;
}

void ls_token_delete(struct ls_token_t *tok) {
  if(!LS_IS_CHAR_TOKEN(tok->type) && tok->value.s) {
    free(tok->value.s);
    tok->value.s = NULL;
  }
}