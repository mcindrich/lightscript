#ifndef __LIGHTSCRIPT_ARRAY_H__
#define __LIGHTSCRIPT_ARRAY_H__

#include <lightscript/var.h>

struct ls_array_t {
  void *c_arr;
  struct ls_var_t *vars;
  size_t size;
};

void ls_array_create(struct ls_array_t *, size_t );
void ls_array_delete(struct ls_array_t *);

#endif