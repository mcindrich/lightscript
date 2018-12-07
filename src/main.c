#include <lightscript.h>
#include <stdio.h>
#include <stdlib.h>

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
  struct ls_var_t var, ref;
  ls_var_create(&var);
  ls_var_create(&ref);

  ls_var_set_name(&var, "name");
  ls_var_set_name(&ref, "ref_var");

  ls_var_set_s8_value(&var, (s8)8);
  ls_var_set_reference_value(&ref, &var);

  //s8 val = ls_var_get_s8_value(&var);
  s8 val = ls_var_get_s8_value(ls_var_get_reference_value(&ref));
  printf("%d\n", val);
  ls_var_delete(&var);
  ls_var_delete(&ref);
#endif
  return main_ret_val;
}