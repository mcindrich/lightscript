#ifndef __LIGHTSCRIPT_ERROR_H__
#define __LIGHTSCRIPT_ERROR_H__

#include <stddef.h>

struct ls_error_t {
  size_t line;
  char* message;
};

void ls_error_create(struct ls_error_t *);
void ls_error_print(struct ls_error_t *);
void ls_error_set(struct ls_error_t *, size_t, const char *);
void ls_error_delete(struct ls_error_t *);

#endif