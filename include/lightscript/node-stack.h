#ifndef __LIGHTSCRIPT_NODE_STACK_H__
#define __LIGHTSCRIPT_NODE_STACK_H__

#include <lightscript/node.h>

struct ls_node_stack_t {
  struct ls_node_t **nodes;
  size_t size;
  size_t count; // used for push and pop because size is a constant ==> speed
};

void ls_node_stack_create(struct ls_node_stack_t *, size_t );
void ls_node_stack_push(struct ls_node_stack_t *, struct ls_node_t *);
struct ls_node_t *ls_node_stack_pop(struct ls_node_stack_t *);
void ls_node_stack_delete(struct ls_node_stack_t *);

#endif