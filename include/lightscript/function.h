#ifndef __LIGHTSCRIPT_FUNCTION_H__
#define __LIGHTSCRIPT_FUNCTION_H__

#include <lightscript/node.h>
#include <lightscript/var-list.h>
#include <lightscript/typedefs.h>

typedef struct ls_var_t (*ls_c_function_t)(struct ls_var_list_t *, 
  struct ls_var_list_t *);

struct ls_function_t {
  struct ls_node_t *node; // if it's a user defined ==> has a node for execution
  ls_c_function_t c_function; // if it's an internal C function ==> has a function pointer
  struct ls_var_list_t *global_vars; // globals
  u8 argc; // only used for speed in user defined functions
};

void ls_function_create(struct ls_function_t *);
void ls_function_set_node(struct ls_function_t *, struct ls_node_t *);
void ls_function_set_c_function(struct ls_function_t *, ls_c_function_t );
struct ls_var_t ls_function_execute(struct ls_function_t *, 
  struct ls_var_list_t *, struct ls_var_list_t *);
void ls_function_delete(struct ls_function_t *);

#endif