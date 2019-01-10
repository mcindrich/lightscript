#include <lightscript/module.h>
#include <lightscript/defines.h>
#include <string.h>
#include <stdlib.h>

void ls_module_create(struct ls_module_t *mod) {
  ls_var_list_create(&mod->vars, 0);
  mod->imported = ls_boolean_false;
}

void ls_module_import(struct ls_module_t *mod) {
  mod->imported = ls_boolean_true;
}

void ls_module_add_var(struct ls_module_t *mod, struct ls_var_t *var) {
  ls_var_list_add_var(&mod->vars, var);
}

void ls_module_load_file(struct ls_module_t *mod, char *fn) {
  // loads the file and executes it to store all the vars into module vars
}

void ls_module_delete(struct ls_module_t *mod) {
  ls_var_list_delete(&mod->vars);
}
