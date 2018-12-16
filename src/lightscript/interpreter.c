#include <lightscript/interpreter.h>
#include <lightscript/function.h>
#include <lightscript/object.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

static char* read_file(char* fn) {
  char* code = NULL;
  size_t fs;
  FILE* file = fopen(fn, "r");
  if(!file) {
    // error
  } else {
    fseek(file, 0, SEEK_END);
    fs = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = (char*) malloc(sizeof(char) * (fs + 1));
    fread(code, sizeof(char), fs, file);
    code[fs] = 0;
    fclose(file);
  }
  return code;
}

struct ls_var_t ls_c_print_func(struct ls_var_list_t *args) {
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
        printf("%s", ls_var_get_boolean_value(var)? "True" : "False");
        break;
      case ls_var_type_reference:
        while(LS_VAR_IS_REFERENCE(var)) {
          var = ls_var_get_reference_value(var);
        }
        printf("Ref '%s' = ", var->name);
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
            printf("%s", ls_var_get_boolean_value(var)? "True" : "False");
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
  printf("\n");
  return ret;
}

struct ls_var_t ls_c_pow_func(struct ls_var_list_t *args) {
  struct ls_var_t ret;
  ls_var_create(&ret); // void ==> no type
  ls_var_set_s32_value(&ret, (int)pow(ls_var_get_s32_value(&args->vars[0]), ls_var_get_s32_value(&args->vars[1])));
  return ret;
}
void ls_interpreter_create(struct ls_interpreter_t *inter, int argc, 
  char **argv) {
  // parse arguments and setup the interpreter for working
  ls_settings_create(&inter->settings);

  // for now only set the file_name to argument given ==> only possible argument
  ls_settings_set_file_name(&inter->settings, argv[1]);
}

int ls_interpreter_execute(struct ls_interpreter_t *inter) {
  char *str = read_file(inter->settings.file_name);

  ls_error_create(&inter->error);
  ls_ast_create(&inter->ast, str, &inter->error);
  free(str);

  if(inter->error.message) {
    // if an err occurs, don't execute
    ls_error_print(&inter->error);
    return 1;
  } else {
    // go and execute root statement
#ifndef PARSER_DEBUGGING
    // test functions
    struct ls_function_t print_f, pow_f;
    // test objects
    struct ls_object_t file_o;
    // vars for functions and objects
    struct ls_var_t print_f_var, pow_f_var, file_o_var;
    ls_var_list_create(&inter->global_vars, 0);

    ls_var_create(&print_f_var);
    ls_var_set_name(&print_f_var, "CPrint");

    ls_var_create(&pow_f_var);
    ls_var_set_name(&pow_f_var, "CPow");

    ls_function_create(&print_f);
    ls_function_set_c_function(&print_f, ls_c_print_func);

    ls_function_create(&pow_f);
    ls_function_set_c_function(&pow_f, ls_c_pow_func);

    ls_var_set_function_value(&print_f_var, &print_f);
    ls_var_set_function_value(&pow_f_var, &pow_f);

    ls_var_list_add_var(&inter->global_vars, &print_f_var);
    ls_var_list_add_var(&inter->global_vars, &pow_f_var);
  
    ls_exec_create(&inter->exec, inter->ast.root);
    ls_exec_set_global_vars(&inter->exec, &inter->global_vars);
    ls_exec_run(&inter->exec);

    ls_var_list_delete(&inter->global_vars);

    //ls_node_delete(&inter->ast.root);
#endif
  }
  return 0;
}

void ls_interpreter_delete(struct ls_interpreter_t *inter) {
  ls_ast_delete(&inter->ast);
  ls_error_delete(&inter->error);
}