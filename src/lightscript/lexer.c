#include <lightscript/lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ls_lex_sl_comment(char **cptr) {
  for(*cptr += 1; **cptr && **cptr != '\n'; ++(*cptr));
  if(!**cptr) {
    --(*cptr);
  }
}

static void ls_lex_ml_comment(char **cptr) {
  for(*cptr += 2; **cptr; ++(*cptr)) {
    if(**cptr == '^' && *(*cptr + 1) == '^') break;
  }
  if(!**cptr) {
    --(*cptr);
    return;
  }
  *cptr += 1;
}

static void ls_lex_pass_string(char **cptr) {
  for(*cptr += 1; **cptr && **cptr != '\"'; ++(*cptr));
}

static void ls_lex_string(struct ls_token_t *tok, char **cptr, size_t ln) {
  char *str_ptr = *cptr+1;
  for(*cptr += 1; **cptr && **cptr != '\"'; ++(*cptr));
  if(!*str_ptr) {
    // err
  } else {
    tok->type = ls_token_type_string;
    tok->value.s = (char*) malloc(sizeof(char) * (*cptr - str_ptr + 1));
    memcpy(tok->value.s, str_ptr, *cptr - str_ptr);
    tok->value.s[*cptr - str_ptr] = 0;
    tok->line_number = ln;
  }
}

static void ls_lex_char_token(struct ls_token_t *tok, char **cptr, size_t ln) {
  // check for double char tokens ==> >=, <= ...
  // large if statement because why not
  char c = **cptr, next = *(*cptr+1);
  if(LS_IS_DOUBLE_TOKEN(c, next)) {
    tok->value.s = (char*) malloc(sizeof(char) * 3);
    tok->value.s[0] = c; tok->value.s[1] = next; tok->value.s[2] = 0;
    tok->type = ls_token_get_word_type(tok->value.s);
    *(cptr) += 1;
  } else {
    tok->type = ls_token_get_char_type(c);
    tok->value.c = c;
  }
  tok->line_number = ln;
}

static void ls_lex_word_token(struct ls_token_t* tok, char *lptr, 
  char *cptr, size_t ln) {
    tok->value.s = (char*) malloc(sizeof(char) * (cptr - lptr + 1));
    memcpy(tok->value.s, lptr, cptr - lptr);
    tok->value.s[cptr - lptr] = 0;
    tok->type = ls_token_get_word_type(tok->value.s);
    tok->line_number = ln;
}

void ls_lexer_create(struct ls_lexer_t *lexer, char *code, 
  struct ls_error_t *err) {
  char *cptr, *lptr;
  size_t token_cnt = 0, max_stack_size = 0, curr_stack_size = 0, line_cnt = 1;

  lexer->tokens = NULL; // if an error occurs
  lexer->token_count = 0;
  lexer->mss = 0;

  // count the tokens for allocation
  for(cptr = code, lptr = code; *cptr; ++cptr) {
    if(LS_IS_TOKEN(*cptr)) {
      if(*cptr == '#') {
        ls_lex_sl_comment(&cptr);
        if(!*cptr) return;
      } else if(*cptr == '^' && *(cptr+1) == '^') {
        ls_lex_ml_comment(&cptr);
      } else if(*cptr == '\"') {
        // string
        ls_lex_pass_string(&cptr);
        if(!*cptr) {
          // err
          ls_error_set(err, line_cnt, "string goes on until the EOF");
          return;
        }
        ++token_cnt;
      } else {
        if(cptr - lptr) {
          ++token_cnt;
        }
        if(!LS_IS_SPACE(*cptr)) {
          ++token_cnt;
          if(LS_IS_DOUBLE_TOKEN(*cptr, *(cptr+1))) {
            ++cptr;
          }
        } else {
          if(*cptr == '\n') {
            ++line_cnt;
          }
        }
      }
      lptr = cptr + 1;
    }
  }

  if(cptr - lptr) {
    ++token_cnt;
  }

  
  if(!token_cnt) return;

  lexer->tokens = (struct ls_token_t*) malloc(sizeof(struct ls_token_t) * 
    token_cnt);
  lexer->token_count = token_cnt;

  token_cnt = 0; // now reuse for token position
  line_cnt = 1;

  // create tokens
  for(cptr = code, lptr = code; *cptr; ++cptr) {
    if(LS_IS_TOKEN(*cptr)) {
      if(*cptr == '#') {
        ls_lex_sl_comment(&cptr);
      } else if(*cptr == '^' && *(cptr+1) == '^') {
        ls_lex_ml_comment(&cptr);
      } else if(*cptr == '\"') {
        // string
        ls_lex_string(&lexer->tokens[token_cnt], &cptr, line_cnt);
        ++token_cnt;
      } else {
        if(cptr - lptr) {
          // words
          // count 'do' and 'end' words for learning the biggest stack size for 
          // later ==> add later
          ls_lex_word_token(&lexer->tokens[token_cnt], lptr, cptr, line_cnt);
          if(lexer->tokens[token_cnt].type == ls_token_type_do_keyword ||
            lexer->tokens[token_cnt].type == ls_token_type_object_keyword) {
            // increase curr_stack_size
            ++curr_stack_size;
            if(max_stack_size < curr_stack_size) {
              max_stack_size = curr_stack_size;
            }
          } else if(lexer->tokens[token_cnt].type 
            == ls_token_type_end_keyword) {
            // decrease curr_stack_size
            --curr_stack_size;
          }
          ++token_cnt;
        }
        if(!LS_IS_SPACE(*cptr)) {
          ls_lex_char_token(&lexer->tokens[token_cnt], &cptr, line_cnt);
          ++token_cnt;
        } else {
          if(*cptr == '\n') {
            ++line_cnt;
          }
        }
      }
      lptr = cptr + 1;
    }
  }

  if(cptr - lptr) {
    ls_lex_word_token(&lexer->tokens[token_cnt], lptr, cptr, line_cnt);
    if(lexer->tokens[token_cnt].type == ls_token_type_end_keyword) {
      // decrease curr_stack_size
      --curr_stack_size;
    }
  }

  lexer->mss = max_stack_size;

  if(curr_stack_size) {
    ls_error_set(err, 0, "stack error");
  }
}

void ls_lexer_delete(struct ls_lexer_t *lexer) {
  if(lexer->tokens) {
    free(lexer->tokens);
  }
}