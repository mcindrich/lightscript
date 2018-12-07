#include <lightscript/token-stack.h>
#include <stdlib.h>

void ls_token_stack_create(struct ls_token_stack_t *stack, size_t size) {
  if(size) {
    stack->toks = (struct ls_token_t*) malloc(sizeof(struct ls_token_t) * 
      size);
  }
  stack->size = size;
  stack->counter = 0;
}

void ls_token_stack_push(struct ls_token_stack_t *stack, 
  struct ls_token_t *tok) {
  if(stack->counter < stack->size) {
    stack->toks[stack->counter] = *tok;
    ++stack->counter;
  } else {
    // err
  }
}

struct ls_token_t* ls_token_stack_top(struct ls_token_stack_t *stack) {
  if(stack->counter > 0) {
    return &stack->toks[stack->counter-1];
  } else {
    return NULL;
  }
}

struct ls_token_t ls_token_stack_pop(struct ls_token_stack_t *stack) {
  struct ls_token_t tok;
  ls_token_create(&tok);

  if(stack->counter > 0) {
    tok = stack->toks[stack->counter - 1];
    --stack->counter;
  } else {
    // err
  }
  return tok;
}

void ls_token_stack_delete(struct ls_token_stack_t *stack) {
  if(stack->size) {
    free(stack->toks);
    stack->size = 0;
    stack->counter = 0;
  }
}