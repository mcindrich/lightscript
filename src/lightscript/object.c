#include <lightscript/object.h>
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

void ls_object_delete(struct ls_object_t *obj) {
  ls_var_list_delete(&obj->object_vars);
}