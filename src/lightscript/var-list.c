#include <lightscript/var-list.h>
#include <lightscript/var.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ls_var_list_create(struct ls_var_list_t *vl, size_t init_size) {
  vl->vars = NULL;
  vl->count = 0;
  vl->size = 0;
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
  for(; i < vl->count; i++) {
    if(strcmp(vl->vars[i].name, name) == 0 && vl->vars[i].type == type) {
      return &vl->vars[i];
    }
  }
  return NULL;
}

struct ls_var_t *ls_var_list_get_var_by_name(struct ls_var_list_t *vl, 
  char *name) {
  size_t i = 0;
  for(; i < vl->count; i++) {
    if(strcmp(vl->vars[i].name, name) == 0) {
      return &vl->vars[i];
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
  }
}