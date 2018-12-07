#ifndef __LIGHTSCRIPT_TOKEN_STACK_H__
#define __LIGHTSCRIPT_TOKEN_STACK_H__

#include <lightscript/token.h>

struct ls_token_stack_t {
  // stack for only token types because string of a token is not needed for shunting yard algorithm
  struct ls_token_t *toks;
  size_t size;
  size_t counter;
};

void ls_token_stack_create(struct ls_token_stack_t *, size_t );
void ls_token_stack_push(struct ls_token_stack_t *, struct ls_token_t * );
struct ls_token_t* ls_token_stack_top(struct ls_token_stack_t *);
struct ls_token_t ls_token_stack_pop(struct ls_token_stack_t *);
void ls_token_stack_delete(struct ls_token_stack_t *);

#endif