#include <lightscript/function.h>
#include <lightscript/var.h>
#include <lightscript/exec.h>

void ls_function_create(struct ls_function_t *func) {
  func->c_function = NULL;
  func->node = NULL;
  func->argc = 0;
}

void ls_function_set_node(struct ls_function_t *func, struct ls_node_t *node) {
  // set from node ==> parse arguments and add them to the locals list
  struct ls_node_t *curr = node->children[0];
  func->node = node;
  while(curr) {
    func->argc++;
    curr = curr->next;
  }
}

void ls_function_set_c_function(struct ls_function_t *func, ls_c_function_t cf) {
  func->c_function = cf;
}

struct ls_var_t ls_function_execute(struct ls_function_t *func, struct ls_var_list_t *globals,
  struct ls_var_list_t *args) {
  struct ls_var_t ret;
  struct ls_node_t *curr;
  u8 argc = 0;

  ls_var_create(&ret);

  if(func->c_function) {
    // execute an internal functions
    ret = func->c_function(globals, args);
  } else {
    curr = func->node->children[0];
    while(curr) {
      curr = curr->next;
      argc++;
    }

    if(args->count == argc) {
      curr = func->node->children[0];
      argc = 0;
      while(curr) {
        ls_var_set_name(&args->vars[argc], curr->token.value.s);
        curr = curr->next;
        argc++;
      }
    }
    // execute created function
    struct ls_exec_t exec;
    ls_exec_create(&exec, func->node->children[1]);
    ls_exec_set_global_vars(&exec, globals);
    // use args list as local variables
    ls_exec_set_local_vars(&exec, args); 
    ls_exec_set_return_var(&exec, &ret);
    ls_exec_run(&exec);
  }
  return ret;
}

void ls_function_delete(struct ls_function_t *func) {
}