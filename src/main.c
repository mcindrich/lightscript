#include <lightscript.h>
#include <lightscript/function.h>
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
#endif
  return main_ret_val;
}