#include <lightscript/interpreter.h>
#include <lightscript/function.h>
#include <lightscript/object.h>
#include <lightscript/modules-build.h>
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
    // call function to add all the modules to the global variables
    ls_var_list_create(&inter->global_vars, 0);
    LS_MODULES_BUILD(&inter->global_vars);
  
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