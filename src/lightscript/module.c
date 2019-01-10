#include <lightscript/module.h>
#include <string.h>
#include <stdlib.h>

void ls_module_create(struct ls_module_t *mod) {
  ls_var_list_create(&mod->vars, 0);
  mod->name = NULL;
}

void ls_module_add_var(struct ls_module_t *mod, struct ls_var_t *var) {
  ls_var_list_add_var(&mod->vars, var);
}

void ls_module_load_file(struct ls_module_t *mod, char *fn) {
  // loads the file and executes it to store all the vars into module vars
}

void ls_module_set_name(struct ls_module_t *mod, char *name) {
  size_t len = strlen(name);
  mod->name = (char *) malloc(sizeof(char) * (len + 1));
  memcpy(mod->name, name, len);
  mod->name[len] = 0;
}

void ls_module_delete(struct ls_module_t *mod) {
  ls_var_list_delete(&mod->vars);
  free(mod->name);
}
