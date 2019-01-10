#ifndef __LIGHTSCRIPT_MODULES_BUILD_H__
#define __LIGHTSCRIPT_MODULES_BUILD_H__

#include <lightscript/var-list.h>
#include <lightscript/module.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

struct ls_var_t ls_stdlib_print_func(struct ls_var_list_t *globals, struct ls_var_list_t *args) {
  struct ls_var_t ret, *var;
  ls_var_create(&ret); // void ==> no type
  size_t i = 0;
  for(; i < args->count; i++) {
    var = &args->vars[i];
    switch(var->type) {
      case ls_var_type_none:
        printf("Nil");
        break;
      case ls_var_type_s32:
        printf("%d", ls_var_get_s32_value(var));
        break;
      case ls_var_type_double:
        printf("%lf", ls_var_get_double_value(var));
        break;
      case ls_var_type_string:
        printf("%s", ls_var_get_string_value(var));
        break;
      case ls_var_type_boolean:
        printf("%s", ls_var_get_ls_boolean_t_value(var)? "True" : "False");
        break;
      case ls_var_type_reference:
        while(LS_VAR_IS_REFERENCE(var)) {
          var = ls_var_get_reference_value(var);
        }
        //printf("Ref '%s' = ", var->name);
        // stupid but will work for now
        switch(var->type) {
          case ls_var_type_none:
            printf("Nil");
            break;
          case ls_var_type_s32:
            printf("%d", ls_var_get_s32_value(var));
            break;
          case ls_var_type_double:
            printf("%lf", ls_var_get_double_value(var));
            break;
          case ls_var_type_string:
            printf("%s", ls_var_get_string_value(var));
            break;
          case ls_var_type_boolean:
            printf("%s", ls_var_get_ls_boolean_t_value(var)? "True" : "False");
            break;
          case ls_var_type_object:
            printf("<< %s >>", var->name);
            break;
          default:
            break;
        }
        break;
      case ls_var_type_object:
        printf("<< %s >>", var->name);
        break;
      default:
        break;
    }
  }
  printf("\n");
  return ret;
}

struct ls_var_t ls_math_pow_func(struct ls_var_list_t *globals, struct ls_var_list_t *args) {
  struct ls_var_t ret;
  ls_var_create(&ret);
  ls_var_set_s32_value(&ret, (int)pow(ls_var_get_s32_value(&args->vars[0]), ls_var_get_s32_value(&args->vars[1])));
  return ret;
}

struct ls_var_t ls_string_strlen_func(struct ls_var_list_t *globals, struct ls_var_list_t *args) {
  struct ls_var_t ret, *str_ptr;
  ls_var_create(&ret);
  if(args->count == 1) {
    str_ptr = ls_var_list_get_var_by_pos(args, 0);
    LS_VAR_GET_FINAL_VALUE(str_ptr);
    if(str_ptr->type == ls_var_type_string) {
      ls_var_set_s32_value(&ret, strlen(ls_var_get_string_value(str_ptr)));
    }
  }
  return ret;
}

struct ls_var_t ls_c_new_func(struct ls_var_list_t *globals, struct ls_var_list_t *args) {
  // function for creating new objects
  // get the object using the name and copy the object
  struct ls_var_t ret, *obj_var = ls_var_list_get_var_by_pos(args, 0);
  ls_var_create(&ret);

  if(obj_var && LS_VAR_IS_REFERENCE(obj_var)) {
    obj_var = ls_var_get_reference_value(obj_var);
  }
  if(obj_var && obj_var->type == ls_var_type_object) {
    ls_var_copy(&ret, obj_var);
  }

  return ret;
}

struct ls_var_t ls_c_file_open_func(struct ls_var_list_t *globals, struct ls_var_list_t *args) {
  struct ls_var_t ret;
  ls_var_create(&ret);
  return ret;
}

void LS_MODULES_BUILD(struct ls_var_list_t *ls) {
  // function to build and experiment with all the modules
  // add your functions in C here
  // load another wanted files by using modules structure
  // all needed modules need to be added to the list in order for them to be able to include
  struct ls_module_t stdlib_mod, string_mod, math_mod;
  struct ls_function_t print_f, pow_f, new_f, strlen_f;
  struct ls_var_t print_f_var, pow_f_var, new_f_var, strlen_f_var, 
    stdlib_mod_var, string_mod_var, math_mod_var;

  // creating variables

  // mod vars
  ls_var_create(&stdlib_mod_var);
  ls_var_set_name(&stdlib_mod_var, "stdlib");

  ls_var_create(&string_mod_var);
  ls_var_set_name(&string_mod_var, "string");

  ls_var_create(&math_mod_var);
  ls_var_set_name(&math_mod_var, "math");
  ///

  // function vars
  ls_var_create(&print_f_var);
  ls_var_set_name(&print_f_var, "Print");

  ls_var_create(&strlen_f_var);
  ls_var_set_name(&strlen_f_var, "StringLength");

  ls_var_create(&pow_f_var);
  ls_var_set_name(&pow_f_var, "MathPow");

  ls_var_create(&new_f_var);
  ls_var_set_name(&new_f_var, "New");
  ///

  /////////////////////

  // creating modules
  ls_module_create(&stdlib_mod);
  ls_module_create(&string_mod);
  ls_module_create(&math_mod);
  ///////////////////

  // creating functions
  ls_function_create(&print_f);
  ls_function_create(&new_f);
  ls_function_create(&pow_f);
  ls_function_create(&strlen_f);
  //////////////////////////

  // functions
  // cprint function
  ls_function_set_c_function(&print_f, ls_stdlib_print_func);

  // new function
  ls_function_set_c_function(&new_f, ls_c_new_func);

  // cpow function
  ls_function_set_c_function(&pow_f, ls_math_pow_func);
  ls_function_set_c_function(&strlen_f, ls_string_strlen_func);
  ////////////////

  // setting variables
  // functions
  ls_var_set_function_value(&print_f_var, &print_f);
  ls_var_set_function_value(&pow_f_var, &pow_f);
  ls_var_set_function_value(&new_f_var, &new_f);
  ls_var_set_function_value(&strlen_f_var, &strlen_f);

  // adding variables to modules
  ls_module_add_var(&stdlib_mod, &print_f_var);
  ls_module_add_var(&string_mod, &strlen_f_var);
  ls_module_add_var(&math_mod, &pow_f_var);
  // create modules
  ls_var_set_module_value(&stdlib_mod_var, &stdlib_mod);
  ls_var_set_module_value(&string_mod_var, &string_mod);
  ls_var_set_module_value(&math_mod_var, &math_mod);
  ///////////////////

  // addding to the appropriate list or module
  // first add modules ==> faster to find later
  ls_var_list_add_var(ls, &stdlib_mod_var);
  ls_var_list_add_var(ls, &string_mod_var);
  ls_var_list_add_var(ls, &math_mod_var);

  // after modules add global variables and functions included by default in the program
  ls_var_list_add_var(ls, &new_f_var);
  ////////////////////////////////
}

#endif