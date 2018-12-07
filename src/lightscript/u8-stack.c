#include <lightscript/u8-stack.h>
#include <stdlib.h>

void ls_u8_stack_create(struct ls_u8_stack_t *stack, size_t size) {
  if(size) {
    stack->values = (u8*) malloc(sizeof(u8) * 
      size);
  }
  stack->size = size;
  stack->counter = 0;
}

void ls_u8_stack_push(struct ls_u8_stack_t *stack, u8 val) {
  if(stack->counter < stack->size) {
    stack->values[stack->counter] = val;
    ++stack->counter;
  }
}

u8 ls_u8_stack_pop(struct ls_u8_stack_t *stack) {
  u8 ret = 0;
  if(stack->counter > 0) {
    ret = stack->values[stack->counter - 1];
    --stack->counter;
  }
  return ret;
}

void ls_u8_stack_delete(struct ls_u8_stack_t *stack) {
  if(stack->size) {
    free(stack->values);
    stack->size = 0;
    stack->counter = 0;
  }
}