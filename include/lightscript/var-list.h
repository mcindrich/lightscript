#ifndef __LIGHTSCRIPT_VAR_LIST_H__
#define __LIGHTSCRIPT_VAR_LIST_H__

#include <stddef.h>

// allocate chunks for variables ==> speed
#define VAR_CHUNK_ALLOC_SIZE 8

enum ls_var_type_t;
struct ls_var_t;

struct ls_var_list_t {
  struct ls_var_t *vars;
  size_t size;
  size_t count;
};

void ls_var_list_create(struct ls_var_list_t *, size_t );
void ls_var_list_debug_print(struct ls_var_list_t *);
void ls_var_list_add_var(struct ls_var_list_t *, struct ls_var_t *);
struct ls_var_t *ls_var_list_get_var_by_name_and_type(struct ls_var_list_t *, 
char *, enum ls_var_type_t);
struct ls_var_t *ls_var_list_get_var_by_name(struct ls_var_list_t *, 
  char *);
void ls_var_list_delete(struct ls_var_list_t *);

#endif