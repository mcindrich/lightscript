#include <lightscript/var.h>
#include <lightscript/function.h>
#include <lightscript/array.h>
#include <lightscript/object.h>
#include <lightscript/module.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// create
void ls_var_create(struct ls_var_t *var) {
  var->name = NULL;
  var->value = NULL;
  var->type = ls_var_type_none;
}
// --------------------------------------------------------------

// copy function

void ls_var_copy(struct ls_var_t *v1, struct ls_var_t *v2) {
  struct ls_object_t temp_obj, *temp_obj_ptr = NULL;
  struct ls_array_t temp_arr, *temp_arr_ptr = NULL;

  switch(v2->type) {
    case ls_var_type_none:
      break;
    case ls_var_type_s8:
      ls_var_set_s8_value(v1, ls_var_get_s8_value(v2));
      break;
    case ls_var_type_u8:
      ls_var_set_u8_value(v1, ls_var_get_u8_value(v2));
      break;
    case ls_var_type_s16:
      ls_var_set_s16_value(v1, ls_var_get_s16_value(v2));
      break;
    case ls_var_type_u16:
      ls_var_set_u16_value(v1, ls_var_get_u16_value(v2));
      break;
    case ls_var_type_s32:
      ls_var_set_s32_value(v1, ls_var_get_s32_value(v2));
      break;
    case ls_var_type_u32:
      ls_var_set_u32_value(v1, ls_var_get_u32_value(v2));
      break;
    case ls_var_type_s64:
      ls_var_set_s64_value(v1, ls_var_get_s64_value(v2));
      break;
    case ls_var_type_u64:
      ls_var_set_u64_value(v1, ls_var_get_u64_value(v2));
      break;
    case ls_var_type_double:
      ls_var_set_double_value(v1, ls_var_get_double_value(v2));
      break;
    case ls_var_type_boolean:
      ls_var_set_ls_boolean_t_value(v1, ls_var_get_ls_boolean_t_value(v2));
      break;
    case ls_var_type_string:
      ls_var_set_string_value(v1, ls_var_get_string_value(v2));
      break;
    case ls_var_type_reference:
      ls_var_set_reference_value(v1, ls_var_get_reference_value(v2));
      break;
    case ls_var_type_function:
      ls_var_set_function_value(v1, ls_var_get_function_value(v2));
      break;
    case ls_var_type_object:
      temp_obj_ptr = ls_var_get_object_value(v2);
      ls_object_create(&temp_obj);
      ls_var_list_copy(&temp_obj.object_vars, &temp_obj_ptr->object_vars);
      ls_var_set_object_value(v1, &temp_obj);
      break;
    case ls_var_type_array:
      break;
    case ls_var_type_module:
      break;
  }
  v1->type = v2->type;
}

// --------------------------------------------------------------

// setters
void ls_var_set_name(struct ls_var_t *var, char *name) {
  size_t len = strlen(name);
  var->name = (char *) malloc(sizeof(char) * (len + 1));
  memcpy(var->name, name, len);
  var->name[len] = 0;
}

void ls_var_set_s8_value(struct ls_var_t *var, s8 val) {
  var->value = (s8 *) malloc(sizeof(s8));
  *((s8 *)var->value) = val;
  var->type = ls_var_type_s8;
}

void ls_var_set_u8_value(struct ls_var_t *var, u8 val) {
  var->value = (u8 *) malloc(sizeof(u8));
  *((u8 *)var->value) = val;
  var->type = ls_var_type_u8;
}

void ls_var_set_s16_value(struct ls_var_t *var, s16 val) {
  var->value = (s16 *) malloc(sizeof(s16));
  *((s16 *)var->value) = val;
  var->type = ls_var_type_s16;
}

void ls_var_set_u16_value(struct ls_var_t *var, u16 val) {
  var->value = (u16 *) malloc(sizeof(u16));
  *((u16 *)var->value) = val;
  var->type = ls_var_type_u16;
}

void ls_var_set_s32_value(struct ls_var_t *var, s32 val) {
  var->value = (s32 *) malloc(sizeof(s32));
  *((s32 *)var->value) = val;
  var->type = ls_var_type_s32;
}

void ls_var_set_u32_value(struct ls_var_t *var, u32 val) {
  var->value = (u32 *) malloc(sizeof(u32));
  *((u32 *)var->value) = val;
  var->type = ls_var_type_u32;
}

void ls_var_set_s64_value(struct ls_var_t *var, s64 val) {
  var->value = (s64 *) malloc(sizeof(s64));
  *((s64 *)var->value) = val;
  var->type = ls_var_type_s64;
}

void ls_var_set_u64_value(struct ls_var_t *var, u64 val) {
  var->value = (u64 *) malloc(sizeof(u64));
  *((u64 *)var->value) = val;
  var->type = ls_var_type_u64;
}

void ls_var_set_double_value(struct ls_var_t *var, double val) {
  var->value = (double *) malloc(sizeof(double));
  *((double *)var->value) = val;
  var->type = ls_var_type_double;
}

void ls_var_set_ls_boolean_t_value(struct ls_var_t *var, ls_boolean_t val) {
  var->value = (ls_boolean_t *) malloc(sizeof(ls_boolean_t));
  *((ls_boolean_t *)var->value) = val;
  var->type = ls_var_type_boolean;
}

void ls_var_set_string_value(struct ls_var_t *var, char* val) {
  size_t len = strlen(val);
  var->value = (char *) malloc(sizeof(char) * (len + 1));
  memcpy((char*)var->value, val, len);
  *((char *)var->value + len) = 0;
  var->type = ls_var_type_string;
}

void ls_var_set_string_concat_value(struct ls_var_t *var, char *s1, char *s2) {
  size_t len1 = strlen(s1), len2 = strlen(s2);
  var->value = (char *) malloc(sizeof(char) * (len1 + len2 + 1));
  memcpy((char*)var->value, s1, len1);
  memcpy((char*)var->value + len1, s2, len2);
  *((char *)var->value + len1 + len2) = 0;
  var->type = ls_var_type_string;
}

void ls_var_set_string_multiply_value(struct ls_var_t *var, char *str, 
  int mul) {
  const size_t len = strlen(str);
  size_t i;
  var->value = (char *) malloc(sizeof(char) * (len * mul + 1));
  memcpy((char*)var->value, str, len);

  // faster multiplication
  for(i = 1; i < mul; i += i) {
     memcpy(var->value + i * len, var->value, i+i > mul? len * (mul - i) : 
      len * i);
  }
  *((char*)var->value + mul * len) = 0;
  var->type = ls_var_type_string;
}

void ls_var_set_reference_value(struct ls_var_t *var, struct ls_var_t *ptr) {
  var->value = (struct ls_var_t **) malloc(sizeof(struct ls_var_t *));
  *((struct ls_var_t **)var->value) = ptr;
  var->type = ls_var_type_reference;
}

void ls_var_set_function_value(struct ls_var_t *var, 
  struct ls_function_t *val) {
  var->value = (struct ls_function_t *) malloc(sizeof(struct ls_function_t));
  *((struct ls_function_t *)var->value) = *val;
  var->type = ls_var_type_function;
}

void ls_var_set_object_value(struct ls_var_t *var, struct ls_object_t *val) {
  var->value = (struct ls_object_t *) malloc(sizeof(struct ls_object_t));
  *((struct ls_object_t *)var->value) = *val;
  var->type = ls_var_type_object;
}

void ls_var_set_array_value(struct ls_var_t *var, struct ls_array_t *val) {
  var->value = (struct ls_array_t *) malloc(sizeof(struct ls_array_t));
  *((struct ls_array_t *)var->value) = *val;
  var->type = ls_var_type_array;
}

void ls_var_set_module_value(struct ls_var_t *var, struct ls_module_t *val) {
  var->value = (struct ls_module_t *) malloc(sizeof(struct ls_module_t));
  *((struct ls_module_t *)var->value) = *val;
  var->type = ls_var_type_module;
}

// --------------------------------------------------------------

// getters
s8 ls_var_get_s8_value(struct ls_var_t *var) {
  return *((s8 *)var->value);
}

u8 ls_var_get_u8_value(struct ls_var_t *var) {
  return *((u8 *)var->value);
}

s16 ls_var_get_s16_value(struct ls_var_t *var) {
  return *((s16 *)var->value);
}

u16 ls_var_get_u16_value(struct ls_var_t *var) {
  return *((u16 *)var->value);
}

s32 ls_var_get_s32_value(struct ls_var_t *var) {
  return *((s32 *)var->value);
}

u32 ls_var_get_u32_value(struct ls_var_t *var) {
  return *((u32 *)var->value);
}

s64 ls_var_get_s64_value(struct ls_var_t *var) {
  return *((s64 *)var->value);
}

u64 ls_var_get_u64_value(struct ls_var_t *var) {
  return *((u64 *)var->value);
}

double ls_var_get_double_value(struct ls_var_t *var) {
  return *((double *)var->value);
}

ls_boolean_t ls_var_get_ls_boolean_t_value(struct ls_var_t *var) {
  return *((ls_boolean_t *)var->value);
}

char* ls_var_get_string_value(struct ls_var_t *var) {
  return (char *)var->value;
}

struct ls_var_t *ls_var_get_reference_value(struct ls_var_t *var) {
  return *((struct ls_var_t  **)var->value);
}

struct ls_function_t *ls_var_get_function_value(struct ls_var_t *var) {
  return (struct ls_function_t *)var->value;
}

struct ls_object_t *ls_var_get_object_value(struct ls_var_t *var) {
  return (struct ls_object_t *)var->value;
}

struct ls_array_t *ls_var_get_array_value(struct ls_var_t *var) {
  return (struct ls_array_t *)var->value; 
}

struct ls_module_t *ls_var_get_module_value(struct ls_var_t *var) {
  return (struct ls_module_t *)var->value;
}

// --------------------------------------------------------------

static void ls_var_check_operator_reference(struct ls_var_t **l, 
  struct ls_var_t **r) {
  // check if its a variable ==> all variables are stored as references aka 
  //          pointers to other vars
  // ==> convert them into normal variables and continue with the operations
  while(LS_VAR_IS_REFERENCE((*l))) {
    *l = ls_var_get_reference_value(*l);
  }
  while(LS_VAR_IS_REFERENCE((*r))) {
    *r = ls_var_get_reference_value(*r);
  }
}

// operators and assign
struct ls_var_t ls_var_operator_add(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);
  
  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    ls_var_set_s32_value(&new, ls_var_get_s32_value(l) + 
      ls_var_get_s32_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    ls_var_set_double_value(&new, ls_var_get_double_value(l) + 
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    ls_var_set_double_value(&new, ls_var_get_s32_value(l) + 
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    ls_var_set_double_value(&new, ls_var_get_double_value(l) + 
      ls_var_get_s32_value(r));
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // concat strings
    ls_var_set_string_concat_value(&new, l->value, r->value);    
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_double_colon(struct ls_var_t *l, 
  struct ls_var_t *r) {
  struct ls_var_t new;
  struct ls_array_t arr;
  u32 start, end, i;
  ls_var_create(&new);
  
  ls_var_check_operator_reference(&l, &r);
  
  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // get left and right operand and create an array from one to another
    start = ls_var_get_s32_value(l), end = ls_var_get_s32_value(r);
    ls_array_create(&arr, end > start && start > 0? end - start : 0);
    for(i = start; i <= end; i++) {
      ls_var_create(ls_array_get_element(&arr, i));
      ls_var_set_s32_value(ls_array_get_element(&arr, i), i);
    }
    ls_var_set_array_value(&new, &arr);
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // permutation of strings ==
    // ls_var_set_string_concat_value(&new, l->value, r->value);    
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_sub(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    ls_var_set_s32_value(&new, ls_var_get_s32_value(l) - 
      ls_var_get_s32_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    ls_var_set_double_value(&new, ls_var_get_double_value(l) -
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    ls_var_set_double_value(&new, ls_var_get_s32_value(l) -
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    ls_var_set_double_value(&new, ls_var_get_double_value(l) -
      ls_var_get_s32_value(r));
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_mul(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);
  
  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    ls_var_set_s32_value(&new, ls_var_get_s32_value(l) * 
      ls_var_get_s32_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    ls_var_set_double_value(&new, ls_var_get_double_value(l) *
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    ls_var_set_double_value(&new, ls_var_get_s32_value(l) *
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    ls_var_set_double_value(&new, ls_var_get_double_value(l) *
      ls_var_get_s32_value(r));
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_STRING(r)) {
    // int string
    ls_var_set_string_multiply_value(&new, ls_var_get_string_value(r),
      ls_var_get_s32_value(l));
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_INT(r)) {
    // string int
    ls_var_set_string_multiply_value(&new, ls_var_get_string_value(l),
      ls_var_get_s32_value(r));
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_div(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    ls_var_set_s32_value(&new, ls_var_get_s32_value(l) / 
      ls_var_get_s32_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    ls_var_set_double_value(&new, ls_var_get_double_value(l) /
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    ls_var_set_double_value(&new, ls_var_get_s32_value(l) /
      ls_var_get_double_value(r));
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    ls_var_set_double_value(&new, ls_var_get_double_value(l) /
      ls_var_get_s32_value(r));
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_mod(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    ls_var_set_s32_value(&new, ls_var_get_s32_value(l) % 
      ls_var_get_s32_value(r));
  } else {

  }
  return new;
}

struct ls_var_t ls_var_operator_obj(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_lt(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    if(ls_var_get_s32_value(l) < ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    if(ls_var_get_double_value(l) < ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    if(ls_var_get_s32_value(l) < ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    if(ls_var_get_double_value(l) < ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // string string
    if(strlen(ls_var_get_string_value(l)) < strlen(ls_var_get_string_value(r))) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_le(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    if(ls_var_get_s32_value(l) <= ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    if(ls_var_get_double_value(l) <= ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    if(ls_var_get_s32_value(l) <= ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    if(ls_var_get_double_value(l) <= ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // string string
    if(strlen(ls_var_get_string_value(l)) <= strlen(ls_var_get_string_value(r))) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_gt(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    if(ls_var_get_s32_value(l) > ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    if(ls_var_get_double_value(l) > ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    if(ls_var_get_s32_value(l) > ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    if(ls_var_get_double_value(l) > ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // string string
    if(strlen(ls_var_get_string_value(l)) > strlen(ls_var_get_string_value(r))) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_ge(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);

  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    if(ls_var_get_s32_value(l) >= ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    if(ls_var_get_double_value(l) >= ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    if(ls_var_get_s32_value(l) >= ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    if(ls_var_get_double_value(l) >= ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // string string
    if(strlen(ls_var_get_string_value(l)) >= strlen(ls_var_get_string_value(r))) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_eq(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);
  
  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    if(ls_var_get_s32_value(l) == ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    if(ls_var_get_double_value(l) == ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    if(ls_var_get_s32_value(l) == ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    if(ls_var_get_double_value(l) == ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // string string
    if(strcmp(ls_var_get_string_value(l), ls_var_get_string_value(r)) == 0) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_ne(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  ls_var_check_operator_reference(&l, &r);
  
  if(LS_VAR_IS_INT(l) && LS_VAR_IS_INT(r)) {
    // int int ==> for now only s32
    if(ls_var_get_s32_value(l) != ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_DOUBLE(r)) {
    // double double
    if(ls_var_get_double_value(l) != ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_INT(l) && LS_VAR_IS_DOUBLE(r)) {
    // int double
    if(ls_var_get_s32_value(l) != ls_var_get_double_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_DOUBLE(l) && LS_VAR_IS_INT(r)) {
    // double int
    if(ls_var_get_double_value(l) != ls_var_get_s32_value(r)) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else if(LS_VAR_IS_STRING(l) && LS_VAR_IS_STRING(r)) {
    // string string
    if(strcmp(ls_var_get_string_value(l), ls_var_get_string_value(r)) != 0) {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_true);
    } else {
      ls_var_set_ls_boolean_t_value(&new, ls_boolean_false);
    }
  } else {
    // err
  }
  return new;
}

struct ls_var_t ls_var_operator_equal(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new; // return value of the equal expression
  struct ls_var_t *ref = ls_var_get_reference_value(l), *last = NULL;
  ls_var_create(&new);

  // if the var already has a value ==> delete it

  while(LS_VAR_IS_REFERENCE(r)) {
    last = r;
    r = ls_var_get_reference_value(r);
  }

  ls_var_set_reference_value(&new, ref);

  if(last) {
    if(ref->value != ls_var_get_reference_value(last)->value) {
      ls_var_delete_value(ref);
      ls_var_set_reference_value(ref, ls_var_get_reference_value(last));
    }
  } else {
    while(LS_VAR_IS_REFERENCE(ref)) {
      ref = ls_var_get_reference_value(ref);
    }
    ls_var_delete_value(ref);
    ref->value = r->value;
    ref->type = r->type;
  }
  return new;
}

struct ls_var_t ls_var_operator_pequal(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_mequal(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_dequal(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_oequal(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_aequal(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_or(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  if(LS_VAR_IS_BOOLEAN(l) && LS_VAR_IS_BOOLEAN(r)) {
    ls_var_set_ls_boolean_t_value(&new, ls_var_get_ls_boolean_t_value(l) ||
      ls_var_get_ls_boolean_t_value(r));
  } else {
    // error ==> operands must be ls_boolean_t type
  }
  return new;
}

struct ls_var_t ls_var_operator_and(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  if(LS_VAR_IS_BOOLEAN(l) && LS_VAR_IS_BOOLEAN(r)) {
    ls_var_set_ls_boolean_t_value(&new, ls_var_get_ls_boolean_t_value(l) &&
      ls_var_get_ls_boolean_t_value(r));
  } else {
    // error ==> operands must be ls_boolean_t type
  }
  return new;
}

struct ls_var_t ls_var_operator_log_or(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

struct ls_var_t ls_var_operator_log_and(struct ls_var_t *l, struct ls_var_t *r) {
  struct ls_var_t new;
  ls_var_create(&new);
  return new;
}

void ls_var_operator_assign(struct ls_var_t *var, struct ls_var_t *value) {
}

// --------------------------------------------------------------

// free

void ls_var_delete_value(struct ls_var_t *var) {
  if(var->value) {
    if(var->type == ls_var_type_array) {
      ls_array_delete(ls_var_get_array_value(var));
    } else if(var->type == ls_var_type_object) {
      ls_object_delete(ls_var_get_object_value(var));
    } else if(var->type == ls_var_type_module) {
      ls_module_delete(ls_var_get_module_value(var));
    }
    free(var->value);
    var->type = ls_var_type_none;
  }
}

void ls_var_delete(struct ls_var_t *var) {
  ls_var_delete_value(var);
  if(var->name) {
    free(var->name);
  }
}