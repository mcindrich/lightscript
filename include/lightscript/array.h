#ifndef __LIGHTSCRIPT_ARRAY_H__
#define __LIGHTSCRIPT_ARRAY_H__

#include <lightscript/var.h>

#define ARRAY_EXTRA_ALLOC_SPACE 20

struct ls_array_t {
  struct ls_var_t *vars;
  size_t size;
};

void ls_array_create(struct ls_array_t *, size_t );
void ls_array_set_element(struct ls_array_t *, size_t, struct ls_var_t *);
struct ls_var_t *ls_array_get_element(struct ls_array_t *, size_t);
void ls_array_delete(struct ls_array_t *);

#endif