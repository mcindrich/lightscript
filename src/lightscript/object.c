#include <lightscript/object.h>
#include <lightscript/function.h>
#include <lightscript/var.h>
#include <stdlib.h>

void ls_object_create(struct ls_object_t *obj) {
  obj->c_object = NULL;
  ls_var_list_create(&obj->object_vars, 0);
}

void ls_object_set_c_object(struct ls_object_t *obj, void *vptr, size_t size) {
  obj->c_object = malloc(size);
}

void ls_object_add_var(struct ls_object_t *obj, struct ls_var_t *var) {
  ls_var_list_add_var(&obj->object_vars, var);
}

struct ls_var_t *ls_object_get_var(struct ls_object_t *obj, char *name) {
  return ls_var_list_get_var_by_name(&obj->object_vars, name);
}

void ls_object_delete(struct ls_object_t *obj) {
  if(obj->c_object) {
    free(obj->c_object);
  }
  ls_var_list_delete(&obj->object_vars);
}