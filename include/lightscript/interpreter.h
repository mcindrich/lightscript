#ifndef __LIGHTSCRIPT_INTERPRETER_H__
#define __LIGHTSCRIPT_INTERPRETER_H__

#include <lightscript/ast.h>
#include <lightscript/error.h>
#include <lightscript/exec.h>
#include <lightscript/settings.h>

struct ls_interpreter_t {
  struct ls_ast_t ast;
  struct ls_error_t error;
  struct ls_exec_t exec; // executor ==> going node by node and interpreting
  struct ls_var_list_t global_vars;
  struct ls_settings_t settings;
};

void ls_interpreter_create(struct ls_interpreter_t *, int, char **);
int ls_interpreter_execute(struct ls_interpreter_t *);
int ls_interpreter_load_from_string(struct ls_interpreter_t*, char *);
void ls_interpreter_delete(struct ls_interpreter_t *);

#endif