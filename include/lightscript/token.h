#ifndef __LIGHTSCRIPT_TOKEN_H__
#define __LIGHTSCRIPT_TOKEN_H__

#include <stddef.h>

enum ls_token_type_t {
  ls_token_type_unknown,

  ls_token_type_word,
  ls_token_type_string,
  ls_token_type_number,

  ls_token_type_var_keyword,
  ls_token_type_def_keyword,
  ls_token_type_object_keyword,
  ls_token_type_new_keyword,
  ls_token_type_return_keyword,
  ls_token_type_if_keyword,
  ls_token_type_elsif_keyword,
  ls_token_type_else_keyword,
  ls_token_type_while_keyword,
  ls_token_type_for_keyword,
  ls_token_type_in_keyword,
  ls_token_type_include_keyword,
  ls_token_type_then_keyword,
  ls_token_type_do_keyword,
  ls_token_type_end_keyword,

  ls_token_type_lbrace,
  ls_token_type_rbrace,
  ls_token_type_lparenth,
  ls_token_type_rparenth,
  ls_token_type_lbracket,
  ls_token_type_rbracket,

  ls_token_type_assign_ops_start, // start of assignment operators
  ls_token_type_equal, // =
  ls_token_type_plus_equal, // +=
  ls_token_type_minus_equal, // -=
  ls_token_type_multiply_equal, // *=
  ls_token_type_divide_equal, // /=
  ls_token_type_and_equal, // &=
  ls_token_type_or_equal, // |=
  ls_token_type_assign_ops_end, // end of assign ops

  ls_token_type_semicolon,
  ls_token_type_comma,
  
  ls_token_type_or_op, // ||
  ls_token_type_and_op, // &&

  ls_token_type_greater_than, // >
  ls_token_type_less_than, // <
  ls_token_type_greater_equal, // >= 
  ls_token_type_less_equal, // <=
  ls_token_type_equal_equal, // == 
  ls_token_type_not_equal, // !=

  ls_token_type_double_colon, // :: ==> loop range
  ls_token_type_plus, // +
  ls_token_type_minus, // -
  ls_token_type_multiply, // *
  ls_token_type_divide, // /
  ls_token_type_percent, // %
  ls_token_type_colon, // : ==> for objects
  ls_token_type_power, // ^
  ls_token_type_log_or, // |
  ls_token_type_log_and, // &

  ls_token_type_increment, // ++ ==> add later
  ls_token_type_decrement, // -- ==> add later
  ls_token_type_exclamation, // ! ==> add later
  ls_token_type_tilde, // ~ ==> add later

  ls_token_type_hash,
  ls_token_type_dollar,
  ls_token_type_at
};

struct ls_token_t {
  enum ls_token_type_t type;
  size_t line_number;
  union {
    char c;
    char* s;
  } value;
};

enum ls_token_type_t ls_token_get_char_type(char );
enum ls_token_type_t ls_token_get_word_type(char *);

void ls_token_create(struct ls_token_t *);
void ls_token_delete(struct ls_token_t *);

#endif