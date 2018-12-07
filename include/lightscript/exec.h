#ifndef __LIGHTSCRIPT_EXEC_H__
#define __LIGHTSCRIPT_EXEC_H__

#include <lightscript/var-list.h>
#include <lightscript/var.h>
#include <lightscript/node.h>

struct ls_exec_t {
  struct ls_var_list_t *global_vars;
  struct ls_var_list_t *local_vars;
  struct ls_var_t *return_var;
  struct ls_node_t *root_node;
};

void ls_exec_create(struct ls_exec_t *, struct ls_node_t *);
void ls_exec_set_global_vars(struct ls_exec_t *, struct ls_var_list_t *);
void ls_exec_set_local_vars(struct ls_exec_t *, struct ls_var_list_t *);
void ls_exec_set_return_var(struct ls_exec_t *, struct ls_var_t *);
void ls_exec_run(struct ls_exec_t *);
void ls_exec_delete(struct ls_exec_t *);

#endif