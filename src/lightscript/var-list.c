#include <lightscript/var-list.h>
#include <lightscript/var.h>
#include <lightscript/module.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls_var_list_create(struct ls_var_list_t *vl, size_t init_size) {
  vl->vars = NULL;
  vl->count = 0;
  vl->size = init_size;
  if(init_size) {
    vl->vars = (struct ls_var_t *) malloc(sizeof(struct ls_var_t) * init_size);
  }
}

void ls_var_list_copy(struct ls_var_list_t *dest, struct ls_var_list_t *src) {
  ls_var_list_delete(dest);

  dest->size = src->size;
  dest->vars = (struct ls_var_t *) malloc(sizeof(struct ls_var_t) * dest->size);

  for(; dest->count < src->count; dest->count++) {
    ls_var_create(&dest->vars[dest->count]);
    ls_var_set_name(&dest->vars[dest->count], src->vars[dest->count].name);
    ls_var_copy(&dest->vars[dest->count], &src->vars[dest->count]);
  }
}

struct ls_var_t *ls_var_list_add_var(struct ls_var_list_t *vl, 
  struct ls_var_t *var) {
  if(vl->count == vl->size) {
    // update size of the list
    vl->size += VAR_CHUNK_ALLOC_SIZE;
    vl->vars = (struct ls_var_t *) realloc(vl->vars, sizeof(struct ls_var_t) * 
      vl->size);
  }
  vl->vars[vl->count] = *var;
  ++vl->count;
  return &vl->vars[vl->count-1];
}

void ls_var_list_debug_print(struct ls_var_list_t *vl) {
  if(vl->count) {
    size_t i;
    for(i = 0; i < vl->count; i++) {
      printf("Var: %s\n", vl->vars[i].name);
    }
  }
}

struct ls_var_t *ls_var_list_get_var_by_name_and_type(struct ls_var_list_t *vl, 
  char *name, enum ls_var_type_t type) {
  size_t i = 0;
  struct ls_var_t *var;
  for(; i < vl->count; i++) {
    var = &vl->vars[i];
    if(strcmp(var->name, name) == 0 && var->type == type) {
      return var;
    }
    if(LS_VAR_IS_MODULE(var) && ls_var_get_module_value(var)->imported) {
      // for now use recursion ==> later add a stack
      var = ls_var_list_get_var_by_name_and_type(
        &ls_var_get_module_value(var)->vars, name, type);
      if(var) return var;
    }
  }
  return NULL;
}

struct ls_var_t *ls_var_list_get_var_by_pos(struct ls_var_list_t *vl, 
  size_t pos) {
  if(pos >= 0 && pos < vl->count) {
    return &vl->vars[pos];
  }
  return NULL;
}

struct ls_var_t *ls_var_list_get_var_by_name(struct ls_var_list_t *vl, 
  char *name) {
  size_t i = 0;
  struct ls_var_t *var;
  for(; i < vl->count; i++) {
    var = &vl->vars[i];
    if(strcmp(var->name, name) == 0) {
      return var;
    }
    if(LS_VAR_IS_MODULE(var) && ls_var_get_module_value(var)->imported) {
      // for now use recursion ==> later add a stack
      var = ls_var_list_get_var_by_name(
        &ls_var_get_module_value(var)->vars, name);
      if(var) return var;
    }
  }
  return NULL;
}

void ls_var_list_delete(struct ls_var_list_t *vl) {
  if(vl->count) {
    size_t i;
    for(i = 0; i < vl->count; i++) {
      ls_var_delete(&vl->vars[i]);
    }
    free(vl->vars);
    vl->size = 0;
    vl->count = 0;
  }
}