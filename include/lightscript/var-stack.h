#ifndef __LIGHTSCRIPT_VAR_STACK_H__
#define __LIGHTSCRIPT_VAR_STACK_H__

#include <lightscript/var.h>

struct ls_var_stack_t {
  struct ls_var_t *vars;
  size_t size;
  size_t counter;
};

void ls_var_stack_create(struct ls_var_stack_t *, size_t );
void ls_var_stack_push(struct ls_var_stack_t *, struct ls_var_t *);
struct ls_var_t ls_var_stack_pop(struct ls_var_stack_t *);
void ls_var_stack_delete(struct ls_var_stack_t *);

#endif