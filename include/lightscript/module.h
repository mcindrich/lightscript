#ifndef __LIGHTSCRIPT_MODULE_H__
#define __LIGHTSCRIPT_MODULE_H__

#include <lightscript/var-list.h>
#include <lightscript/typedefs.h>

struct ls_module_t {
  struct ls_var_list_t vars;
  ls_boolean_t imported;
};

void ls_module_create(struct ls_module_t *);
void ls_module_import(struct ls_module_t *);
void ls_module_add_var(struct ls_module_t *, struct ls_var_t *);
void ls_module_load_file(struct ls_module_t *, char *);
void ls_module_delete(struct ls_module_t *);

#endif