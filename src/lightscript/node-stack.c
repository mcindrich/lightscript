#include <lightscript/node-stack.h>
#include <stdlib.h>
#include <stdio.h>

void ls_node_stack_create(struct ls_node_stack_t *stack, size_t size) {
  if(size) {
    stack->nodes = (struct ls_node_t**) malloc(sizeof(struct ls_node_t*) * 
      size);
  }
  stack->size = size;
  stack->count = 0;
}

void ls_node_stack_push(struct ls_node_stack_t *stack, struct ls_node_t *node) {
  if(stack->count < stack->size) {
    stack->nodes[stack->count] = node;
    ++stack->count;
  }
}

struct ls_node_t *ls_node_stack_pop(struct ls_node_stack_t *stack) {
  struct ls_node_t* node_ptr = NULL;
  if(stack->count > 0) {
    node_ptr = stack->nodes[stack->count - 1];
    --stack->count;
  }
  return node_ptr;
}

struct ls_node_t *ls_node_stack_top(struct ls_node_stack_t *stack) {
  struct ls_node_t* node_ptr = NULL;
  if(stack->count > 0) {
    node_ptr = stack->nodes[stack->count - 1];
  }
  return node_ptr;
}

void ls_node_stack_delete(struct ls_node_stack_t *stack) {
  if(stack->size) {
    free(stack->nodes);
    stack->size = 0;
    stack->count = 0;
  }
}