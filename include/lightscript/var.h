#ifndef __LIGHTSCRIPT_VAR_H__
#define __LIGHTSCRIPT_VAR_H__

#include <lightscript/typedefs.h>
#include <lightscript/defines.h>

struct ls_function_t;
struct ls_object_t;
struct ls_array_t;
struct ls_module_t;

enum ls_var_type_t {
  ls_var_type_none,
  ls_var_type_s8,
  ls_var_type_u8,
  ls_var_type_s16,
  ls_var_type_u16,
  ls_var_type_s32,
  ls_var_type_u32,
  ls_var_type_s64,
  ls_var_type_u64,
  ls_var_type_double,
  ls_var_type_boolean,
  ls_var_type_string,
  ls_var_type_reference,
  ls_var_type_function,
  ls_var_type_object,
  ls_var_type_array,
  ls_var_type_module
};

struct ls_var_t {
  char *name;
  void *value;
  enum ls_var_type_t type;
};

void ls_var_create(struct ls_var_t *);
void ls_var_set_name(struct ls_var_t *, char *);
void ls_var_copy(struct ls_var_t *, struct ls_var_t *);

void ls_var_set_s8_value(struct ls_var_t *, s8);
void ls_var_set_u8_value(struct ls_var_t *, u8);
void ls_var_set_s16_value(struct ls_var_t *, s16);
void ls_var_set_u16_value(struct ls_var_t *, u16);
void ls_var_set_s32_value(struct ls_var_t *, s32);
void ls_var_set_u32_value(struct ls_var_t *, u32);
void ls_var_set_s64_value(struct ls_var_t *, s64);
void ls_var_set_u64_value(struct ls_var_t *, u64);
void ls_var_set_double_value(struct ls_var_t *, double);
void ls_var_set_ls_boolean_t_value(struct ls_var_t *, ls_boolean_t);
void ls_var_set_string_value(struct ls_var_t *, char *);
void ls_var_set_string_concat_value(struct ls_var_t *, char *, char *);
void ls_var_set_string_multiply_value(struct ls_var_t *, char *, int);
void ls_var_set_reference_value(struct ls_var_t *, struct ls_var_t *);
void ls_var_set_function_value(struct ls_var_t *, struct ls_function_t *);
void ls_var_set_object_value(struct ls_var_t *, struct ls_object_t *);
void ls_var_set_array_value(struct ls_var_t *, struct ls_array_t *);
void ls_var_set_module_value(struct ls_var_t *, struct ls_module_t *);

s8 ls_var_get_s8_value(struct ls_var_t *);
u8 ls_var_get_u8_value(struct ls_var_t *);
s16 ls_var_get_s16_value(struct ls_var_t *);
u16 ls_var_get_u16_value(struct ls_var_t *);
s32 ls_var_get_s32_value(struct ls_var_t *);
u32 ls_var_get_u32_value(struct ls_var_t *);
s64 ls_var_get_s64_value(struct ls_var_t *);
u64 ls_var_get_u64_value(struct ls_var_t *);
double ls_var_get_double_value(struct ls_var_t *);
ls_boolean_t ls_var_get_ls_boolean_t_value(struct ls_var_t *);
char* ls_var_get_string_value(struct ls_var_t *);
struct ls_var_t *ls_var_get_reference_value(struct ls_var_t *);
struct ls_function_t *ls_var_get_function_value(struct ls_var_t *);
struct ls_object_t *ls_var_get_object_value(struct ls_var_t *);
struct ls_array_t *ls_var_get_array_value(struct ls_var_t *);
struct ls_module_t *ls_var_get_module_value(struct ls_var_t *);

struct ls_var_t ls_var_operator_add(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_sub(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_mul(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_div(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_mod(struct ls_var_t *, struct ls_var_t *);

// object operator
struct ls_var_t ls_var_operator_obj(struct ls_var_t *, struct ls_var_t *);

struct ls_var_t ls_var_operator_lt(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_le(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_gt(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_ge(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_eq(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_ne(struct ls_var_t *, struct ls_var_t *);

struct ls_var_t ls_var_operator_equal(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_pequal(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_mequal(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_dequal(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_oequal(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_aequal(struct ls_var_t *, struct ls_var_t *);


struct ls_var_t ls_var_operator_log_or(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_log_and(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_or(struct ls_var_t *, struct ls_var_t *);
struct ls_var_t ls_var_operator_and(struct ls_var_t *, struct ls_var_t *);

void ls_var_operator_assign(struct ls_var_t *, struct ls_var_t *);

void ls_var_delete_value(struct ls_var_t *);
void ls_var_delete(struct ls_var_t *);

#endif