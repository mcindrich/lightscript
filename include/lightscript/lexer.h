#ifndef __LIGHTSCRIPT_LEXER_H__
#define __LIGHTSCRIPT_LEXER_H__

#include <stddef.h>
#include <lightscript/defines.h>
#include <lightscript/token.h>
#include <lightscript/error.h>

struct ls_lexer_t {
  struct ls_token_t *tokens;
  size_t token_count;
  size_t mss; // max stack size
};

void ls_lexer_create(struct ls_lexer_t *, char *, struct ls_error_t *);
void ls_lexer_delete(struct ls_lexer_t *);

#endif