#ifndef __LIGHTSCRIPT_OBJECT_H__
#define __LIGHTSCRIPT_OBJECT_H__

#include <lightscript/var-list.h>

struct ls_object_t {
  void *c_object;
  struct ls_var_list_t object_vars;
};

void ls_object_create(struct ls_object_t *);
void ls_object_set_c_object(struct ls_object_t *, void *, size_t );
void ls_object_add_var(struct ls_object_t *, struct ls_var_t *);
void ls_object_delete(struct ls_object_t *);

#endif