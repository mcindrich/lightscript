#ifndef __LIGHTSCRIPT_NODE_H__
#define __LIGHTSCRIPT_NODE_H__

#include <lightscript/token.h>
#include <stddef.h>

enum ls_node_type_t {
  ls_node_type_none, // unknown
  ls_node_type_expression, // 5 + 2
  ls_node_type_assign_statement, // a = 100 + ...
  ls_node_type_array_init, // arr[200] = 100
  ls_node_type_array_element, // arr[200]
  ls_node_type_array, // [100, 200]
  ls_node_type_object_definition, // object ...
  ls_node_type_object_value, // new Obj(100)
  ls_node_type_object_property, // obj:value || obj:fun() || obj:obj2:aa
  ls_node_type_function_definition, // def main() ...
  ls_node_type_function_argument, // def foo(arg) ...
  ls_node_type_function_call, // func(199);
  ls_node_type_if_statement, // if (expr) ...
  ls_node_type_else_statement, // else ...
  ls_node_type_while_statement, // while (expr) ..
  ls_node_type_for_statement, // for expr in expr do...
  ls_node_type_include_statement, // include __module__
  ls_node_type_return_statement // return expr
};

struct ls_node_t {
  struct ls_token_t token;
  enum ls_node_type_t type;
  struct ls_node_t** children;
  struct ls_node_t* next;
  size_t children_count;
};

void ls_node_create(struct ls_node_t **, size_t, enum ls_node_type_t, struct ls_token_t *);
void ls_node_delete(struct ls_node_t **);

#endif