#ifndef __LIGHTSCRIPT_U8_STACK_H__
#define __LIGHTSCRIPT_U8_STACK_H__

#include <lightscript/typedefs.h>
#include <lightscript/defines.h>

struct ls_u8_stack_t {
  u8 *values;
  size_t size;
  size_t counter; // used for push and pop because size is a constant ==> speed
};

void ls_u8_stack_create(struct ls_u8_stack_t *, size_t);
void ls_u8_stack_push(struct ls_u8_stack_t *, u8);
u8 ls_u8_stack_pop(struct ls_u8_stack_t *);
void ls_u8_stack_delete(struct ls_u8_stack_t *);

#endif