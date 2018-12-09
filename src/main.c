#include <lightscript.h>
#include <lightscript/function.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef MAIN_DEBUGGING

struct ls_var_t file_new(struct ls_var_list_t *args) {
  struct ls_var_t ret;
  ls_var_create(&ret);
  return ret;
}

struct ls_var_t file_close(struct ls_var_list_t *args) {
  struct ls_var_t ret;
  ls_var_create(&ret);
  return ret;
}

struct ls_var_t file_read(struct ls_var_list_t *args) {
  struct ls_var_t ret;
  ls_var_create(&ret);
  return ret;
}

#endif

int main(int argc, char **argv) {
  int main_ret_val = 0;
#ifndef MAIN_DEBUGGING
  
  struct ls_interpreter_t inter;
  ls_interpreter_create(&inter, argc, argv);
  // for now only code in file execution 
  // later add the support for line by line command interpreting
  // something like if(inter->interactive) get line, parse, execute
  //                else get file and execute
  main_ret_val = ls_interpreter_execute(&inter);

  if(!main_ret_val) {
    // free resources
    ls_interpreter_delete(&inter);
  }
#else
  struct ls_function_t file_new_func, file_close_func, file_read_func;
  struct ls_var_t file_new_var, file_close_var, file_read_var;

  ls_function_create(&file_new_func);
  ls_function_create(&file_close_func);
  ls_function_create(&file_read_func);

  ls_var_create(&file_new_var);
  ls_var_create(&file_close_var);
  ls_var_create(&file_read_var);

  // file = new File("file.txt"); ==> creates an object
  // str = file:read(); // calls read function from object variables
  // file:close();
  ls_var_set_name(&file_new_var, "__new__");
  ls_var_set_name(&file_close_var, "close");
  ls_var_set_name(&file_read_var, "read");

  ls_function_set_c_function(&file_new_func, file_new);
  ls_function_set_c_function(&file_close_func, file_close);
  ls_function_set_c_function(&file_read_func, file_read);


#endif
  return main_ret_val;
}