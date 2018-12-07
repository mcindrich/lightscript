#include <lightscript/var-stack.h>
#include <stdlib.h>

void ls_var_stack_create(struct ls_var_stack_t *stack, size_t size) {
  if(size) {
    stack->vars = (struct ls_var_t*) malloc(sizeof(struct ls_var_t) * 
      size);
  }
  stack->size = size;
  stack->counter = 0;
}

void ls_var_stack_push(struct ls_var_stack_t *stack, struct ls_var_t *var) {
  if(stack->counter < stack->size) {
    stack->vars[stack->counter] = *var;
    ++stack->counter;
  }
}

struct ls_var_t ls_var_stack_pop(struct ls_var_stack_t *stack) {
  struct ls_var_t ret;
  ls_var_create(&ret);

  if(stack->counter > 0) {
    ret = stack->vars[stack->counter - 1];
    --stack->counter;
  }

  return ret;
}

void ls_var_stack_delete(struct ls_var_stack_t *stack) {
  if(stack->size) {
    free(stack->vars);
    stack->size = 0;
    stack->counter = 0;
  }
}