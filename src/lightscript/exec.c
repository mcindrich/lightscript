#include <lightscript/exec.h>
#include <lightscript/node-stack.h>
#include <lightscript/var-stack.h>
#include <lightscript/defines.h>
#include <lightscript/module.h>
#include <lightscript/function.h>
#include <lightscript/array.h>
#include <lightscript/object.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Function definitions
static struct ls_var_t ls_exec_obj_node(struct ls_exec_t *, 
  struct ls_node_t *);
static struct ls_var_t ls_exec_array_node(struct ls_exec_t *, 
  struct ls_node_t *);
static struct ls_var_t ls_exec_array_element_node(struct ls_exec_t *, 
  struct ls_node_t *);
static struct ls_var_t ls_exec_expr_stack(struct ls_exec_t *, 
  struct ls_node_t *);
struct ls_var_t ls_exec_function_call_statement(struct ls_exec_t *, 
  struct ls_node_t *);

// function to return variable pointer ==> checks all modules and variables
struct ls_var_t *ls_exec_get_var_from_list(struct ls_var_list_t *, char *);
////////////////////////////////////////////////////////////////////////////////

void ls_exec_create(struct ls_exec_t *exec, struct ls_node_t *root_node) {
  exec->root_node = root_node;
  exec->global_vars = NULL;
  exec->local_vars = NULL;
  exec->return_var = NULL;
}

void ls_exec_set_global_vars(struct ls_exec_t *exec, struct ls_var_list_t *ls) {
  exec->global_vars = ls;
}

void ls_exec_set_local_vars(struct ls_exec_t *exec, struct ls_var_list_t *ls) {
  exec->local_vars = ls;
}

void ls_exec_set_return_var(struct ls_exec_t *exec, struct ls_var_t *ret) {
  exec->return_var = ret;
}

static void ls_exec_debug_print_var(struct ls_var_t *var) {
  struct ls_array_t *temp_arr;
  size_t i;

  switch(var->type) {
    case ls_var_type_none:
      printf("NONE\n");
      break;
    case ls_var_type_s32:
      printf("INT: '%d'\n", ls_var_get_s32_value(var));
      break;
    case ls_var_type_double:
      printf("DOUBLE: '%lf'\n", ls_var_get_double_value(var));
      break;
    case ls_var_type_string:
      printf("STRING: '%s'\n", ls_var_get_string_value(var));
      break;
    case ls_var_type_array:
      printf("Array: \n");
      temp_arr = ls_var_get_array_value(var);
      for(i = 0; i < temp_arr->size; i++) {
        ls_exec_debug_print_var(&temp_arr->vars[i]);
      }
      break;
    default:
      printf("OTHER TYPE\n");
      break;
  }
}

struct ls_var_t ls_exec_expr_stack(struct ls_exec_t *exec,
  struct ls_node_t *node) {
  struct ls_node_stack_t node_stack, op_stack;
  struct ls_var_stack_t var_stack;
  struct ls_var_t ret_var, temp_var, left ,right, res, *list_var = NULL;
  enum ls_token_type_t tt;
  struct ls_node_t *current_node = node, *pop_node = NULL, *op_node = NULL;

  size_t *op_counters = (size_t *) calloc(100, sizeof(size_t));

  ls_var_create(&ret_var);
  ls_var_create(&temp_var);
  
  // needs to be precounted ==> TODO
  ls_node_stack_create(&node_stack, 100);
  ls_node_stack_create(&op_stack, 100);
  ls_var_stack_create(&var_stack, 100);

  while(current_node || node_stack.count) {
    while(current_node) {
      ls_node_stack_push(&node_stack, current_node);
      if(LS_IS_VALID_EXPR_NODE(current_node)) {
        ls_node_stack_push(&op_stack, current_node);
      }
      current_node = LS_IS_VALID_EXPR_NODE(current_node)? 
        current_node->children[0] : NULL;
    }
    //printf("Size: %d\n", op_stack.count);
    pop_node = ls_node_stack_pop(&node_stack);
    tt = pop_node->token.type;

    if(LS_IS_VALID_EXPR_NODE(pop_node)) {
      // operator ==> switch side
      current_node = pop_node->children[1];
    } else {
      // operand
      if(pop_node->type == ls_node_type_function_call) {
        temp_var = ls_exec_function_call_statement(exec, pop_node);
      } else if(pop_node->type == ls_node_type_array) {
        // parse array and set it to the temp variable
        temp_var = ls_exec_array_node(exec, pop_node);
      } else if(pop_node->type == ls_node_type_array_element) {
        // parse array and set it to the temp variable
        temp_var = ls_exec_array_element_node(exec, pop_node);
      } else if(pop_node->type == ls_node_type_object_property) {
        temp_var = ls_exec_obj_node(exec, pop_node);
      } else {
        if(tt == ls_token_type_number) {
          double val = atof(pop_node->token.value.s);
          // check if it's an integer ==> TODO: all int types
          // search for '.' and if it exists ==> it's a double
          char* cont = strchr(pop_node->token.value.s, '.');
          if(!cont) {
            ls_var_set_s32_value(&temp_var, (s32)val);
          } else {
            ls_var_set_double_value(&temp_var, val);
          }
        } else if(tt == ls_token_type_string) {
          ls_var_set_string_value(&temp_var, pop_node->token.value.s);
        } else if(tt == ls_token_type_word) {
          // word
          if(exec->local_vars) {
            // get local variable first
            list_var = ls_var_list_get_var_by_name(exec->local_vars, 
              pop_node->token.value.s);
            if(!list_var) {
              // get global variable
              list_var = ls_var_list_get_var_by_name(exec->global_vars, 
                pop_node->token.value.s);
            }
          } else {
            // get global variable
            list_var = ls_var_list_get_var_by_name(exec->global_vars, 
              pop_node->token.value.s);
          }
          if(list_var) {
            ls_var_set_reference_value(&temp_var, list_var);
          } else {
            // create var
            ls_var_create(&temp_var);
            ls_var_set_name(&temp_var, pop_node->token.value.s);
            if(exec->local_vars) {
              list_var = ls_var_list_add_var(exec->local_vars, &temp_var);
            } else {
              list_var = ls_var_list_add_var(exec->global_vars, &temp_var);
            }
            ls_var_create(&temp_var);
            ls_var_set_reference_value(&temp_var, list_var);
          }
        }
      }
      ls_var_stack_push(&var_stack, &temp_var);
      ++op_counters[op_stack.count];

      while(op_counters[op_stack.count] == 2) {
        op_node = ls_node_stack_pop(&op_stack);
        tt = op_node->token.type;

        right = ls_var_stack_pop(&var_stack);
        left = ls_var_stack_pop(&var_stack);

        if(tt == ls_token_type_equal) {
          res = ls_var_operator_equal(&left, &right);
        } else if(tt == ls_token_type_plus) {
          res = ls_var_operator_add(&left, &right);
        } else if(tt == ls_token_type_minus) {
          res = ls_var_operator_sub(&left, &right);
        } else if(tt == ls_token_type_multiply) {
          res = ls_var_operator_mul(&left, &right);
        } else if(tt == ls_token_type_divide) {
          res = ls_var_operator_div(&left, &right);
        } else if(tt == ls_token_type_percent) {
          res = ls_var_operator_mod(&left, &right);
        }  else if(tt == ls_token_type_less_than) {
          res = ls_var_operator_lt(&left, &right);
        } else if(tt == ls_token_type_less_equal) {
          res = ls_var_operator_le(&left, &right);
        } else if(tt == ls_token_type_greater_than) {
          res = ls_var_operator_gt(&left, &right);
        } else if(tt == ls_token_type_greater_equal) {
          res = ls_var_operator_ge(&left, &right);
        } else if(tt == ls_token_type_equal_equal) {
          res = ls_var_operator_eq(&left, &right);
        } else if(tt == ls_token_type_not_equal) {
          res = ls_var_operator_ne(&left, &right);
        } else if(tt == ls_token_type_and_op) {
          res = ls_var_operator_and(&left, &right);
        } else if(tt == ls_token_type_or_op) {
          res = ls_var_operator_or(&left, &right);
        } else if(tt == ls_token_type_double_colon) {
          res = ls_var_operator_double_colon(&left, &right);
        }
        ls_var_delete(&left);
        if(tt != ls_token_type_equal || LS_VAR_IS_REFERENCE((&right))) {
          // when var is assigned ==> don't copy the variable, just assign void*
          // unless it's a reference ==> then just delete it
          ls_var_delete(&right);
        }

        ls_var_stack_push(&var_stack, &res);

        op_counters[op_stack.count + 1] = 0;
        ++op_counters[op_stack.count];
      }
      current_node = NULL;
    }
  }

  ret_var = ls_var_stack_pop(&var_stack);

  free(op_counters);
  ls_node_stack_delete(&node_stack);
  ls_node_stack_delete(&op_stack);
  ls_var_stack_delete(&var_stack);
  return ret_var;
}

void ls_exec_include_statement(struct ls_exec_t *exec, struct ls_node_t *node) {
  struct ls_var_t *list_var = ls_var_list_get_var_by_name(exec->global_vars, node->token.value.s);
  if(LS_VAR_IS_MODULE(list_var)) {
    ls_var_get_module_value(list_var)->imported = ls_boolean_true;
  }
}

void ls_exec_function_definition_statement(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_function_t func;
  struct ls_var_t func_var;

  ls_var_create(&func_var);
  ls_var_set_name(&func_var, node->token.value.s);

  ls_function_create(&func);
  ls_function_set_node(&func, node);

  ls_var_set_function_value(&func_var, &func);
  if(exec->local_vars) {
    // can't do ==> no local functions allowed ==> my defined rule
  } else {
    // add the function to the globals
    ls_var_list_add_var(exec->global_vars, &func_var);
  }
}

void ls_exec_object_definition_statement(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_object_t obj;
  struct ls_var_t obj_var;
  struct ls_exec_t exec2;

  ls_var_create(&obj_var);
  ls_var_set_name(&obj_var, node->token.value.s);

  ls_object_create(&obj);

  ls_exec_create(&exec2, node->children[0]);
  ls_exec_set_global_vars(&exec2, &obj.object_vars);
  ls_exec_run(&exec2);

  ls_var_set_object_value(&obj_var, &obj);

  if(exec->local_vars) {
    ls_var_list_add_var(exec->local_vars, &obj_var);
  } else {
    ls_var_list_add_var(exec->global_vars, &obj_var);
  }
}

struct ls_var_t *ls_exec_get_var_from_list(struct ls_var_list_t *ls, 
  char *name) {
  struct ls_var_t *var = NULL;
  size_t i;
  for(i = 0; i < ls->count; i++) {
    var = &ls->vars[i];
    if(strcmp(var->name, name) == 0) return var;
    if(LS_VAR_IS_MODULE(var)) return ls_exec_get_var_from_list(&ls_var_get_module_value(var)->vars, name);
  }
  return NULL;
}

struct ls_var_t ls_exec_function_call_statement(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_var_t ret_var, temp_var;
  struct ls_var_list_t args;

  ls_var_list_create(&args, 0);
  ls_var_create(&ret_var);

  struct ls_var_t *func = ls_var_list_get_var_by_name(
    exec->global_vars, node->token.value.s);
  //printf("Calling func %s\n",  node->token.value.s);
  if(func) {
    if(node->children[0]) {
      if(node->children[0]->token.type == ls_token_type_comma) {
        // count
        struct ls_node_stack_t node_stack;
        struct ls_node_t *current_node = node->children[0], *pop_node = NULL, *left = NULL, *right = NULL;

        ls_node_stack_create(&node_stack, 255);
        
        while((current_node && current_node->token.type == ls_token_type_comma) || node_stack.count) {
          while(current_node && current_node->token.type == ls_token_type_comma) {
            ls_node_stack_push(&node_stack, current_node);
            if(current_node->children)
              current_node = current_node->children[0];
            else current_node = NULL;
          }
          pop_node = ls_node_stack_pop(&node_stack);
          left = pop_node->children[0];
          right = pop_node->children[1];
          if(left->token.type != ls_token_type_comma) {
            temp_var = ls_exec_expr_stack(exec, left);
            ls_var_list_add_var(&args, &temp_var);
          }
          temp_var = ls_exec_expr_stack(exec, right);
          ls_var_list_add_var(&args, &temp_var);
          if(pop_node->children)
            current_node = pop_node->children[1];
          else current_node = NULL;
        }

        ls_node_stack_delete(&node_stack);
      } else {
        // one argument
        temp_var = ls_exec_expr_stack(exec, node->children[0]);
        ls_var_list_add_var(&args, &temp_var);
      }
    }
    LS_VAR_GET_FINAL_VALUE(func);
    //printf("Final func: %s\n", func->name);
    struct ls_function_t *func_ptr = ls_var_get_function_value(func);
    ret_var = ls_function_execute(func_ptr, exec->global_vars, &args);
  } else {
    // function does not exist
  }
  ls_var_list_delete(&args);
  return ret_var;
}

struct ls_var_t ls_exec_array_node(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_var_t arr_var, temp_var;
  struct ls_array_t arr;
  struct ls_node_stack_t node_stack;
  struct ls_node_t *current_node = node->children[0], *pop_node = NULL, *left = NULL, *right = NULL;
  ls_boolean_t set = ls_boolean_false;
  // needs to be precounted ==> TODO
  ls_node_stack_create(&node_stack, 10);
  u8 cnt = -1;

  // comma counting ==> arrays
  while(current_node || node_stack.count) {
    if(!set) {
      cnt = 0;
      set = ls_boolean_true;
    }
    while(current_node && current_node->token.type == ls_token_type_comma) {
      ls_node_stack_push(&node_stack, current_node);
      ++cnt; // count commas and at the end add one
      if(current_node->children)
        current_node = current_node->children[0];
      else current_node = NULL;
    }
    pop_node = ls_node_stack_pop(&node_stack);
    if(pop_node && pop_node->children)
      current_node = pop_node->children[1];
    else current_node = NULL;
  }
  ++cnt;

  // create an array with a stack
  ls_var_create(&arr_var);
  ls_array_create(&arr, cnt);
  if(cnt) {
    node_stack.count = 0; // flush the stack ==> start again
    cnt = 0;
    current_node = node->children[0];

    while(current_node || node_stack.count) {
      while(current_node && current_node->token.type == ls_token_type_comma) {
        ls_node_stack_push(&node_stack, current_node);
        if(current_node->children)
          current_node = current_node->children[0];
        else current_node = NULL;
      }
      pop_node = ls_node_stack_pop(&node_stack);
      if(pop_node) {
        left = pop_node->children[0];
        right = pop_node->children[1];
        if(left->token.type != ls_token_type_comma) {
          temp_var = ls_exec_expr_stack(exec, left);
          ls_array_set_element(&arr, cnt, &temp_var);
          cnt++;
          pop_node = NULL;
        }
        temp_var = ls_exec_expr_stack(exec, right);
        ls_array_set_element(&arr, cnt, &temp_var);
        cnt++;
      } else {
        // needs a fix ==> TODO
        if(cnt < arr.size) {
          temp_var = ls_exec_expr_stack(exec, current_node);
          ls_array_set_element(&arr, cnt, &temp_var);
        }
      }
      if(pop_node && pop_node->children)
        current_node = pop_node->children[1];
      else current_node = NULL;
    }
  }
  ls_var_set_array_value(&arr_var, &arr);
  ls_node_stack_delete(&node_stack);
  return arr_var;
}

struct ls_var_t ls_exec_array_element_node(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_var_t ret_var, 
    arr_var = ls_exec_array_node(exec, node->children[0]), *arr_var_ptr = NULL, 
    *temp_var_ptr = NULL, expr_res, *curr_pos = NULL;
  struct ls_array_t *arr_ptr = NULL, 
    *arr_pos = ls_var_get_array_value(&arr_var);
  size_t i;

  ls_var_create(&ret_var);

  // get global or local array
  if(exec->local_vars) {
    // get local variable first
    arr_var_ptr = ls_var_list_get_var_by_name(exec->local_vars, 
      node->token.value.s);
    if(!arr_var_ptr) {
      // get global variable
      arr_var_ptr = ls_var_list_get_var_by_name(exec->global_vars, 
        node->token.value.s);
    }
  } else {
    // get global variable
    arr_var_ptr = ls_var_list_get_var_by_name(exec->global_vars, 
      node->token.value.s);
  }
  if(!arr_var_ptr) {
  } else {
    LS_VAR_GET_FINAL_VALUE(arr_var_ptr);
    arr_ptr = ls_var_get_array_value(arr_var_ptr);
  }
  // for now only one dimension array
  curr_pos = &arr_pos->vars[0];
  LS_VAR_GET_FINAL_VALUE(curr_pos);
  temp_var_ptr = ls_array_get_element(arr_ptr, ls_var_get_s32_value(curr_pos));
  if(!temp_var_ptr) {
    // out of bounds
    
  } else {
    ls_var_set_reference_value(&ret_var, temp_var_ptr);
  }
  ls_var_delete(&arr_var);
  return ret_var;
}

struct ls_var_t ls_exec_obj_node(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_var_t ret, *list_var = NULL;
  struct ls_object_t *obj_ptr = NULL;
  char *obj_name = node->children[0]->token.value.s;
  ls_var_create(&ret);

  // get global or local array
  if(exec->local_vars) {
    // get local variable first
    list_var = ls_var_list_get_var_by_name(exec->local_vars, 
      obj_name);
    if(!list_var) {
      // get global variable
      list_var = ls_var_list_get_var_by_name(exec->global_vars, 
        obj_name);
    }
  } else {
    // get global variable
    list_var = ls_var_list_get_var_by_name(exec->global_vars, 
      obj_name);
  }
  if(!list_var) {

  } else {
    obj_ptr = ls_var_get_object_value(list_var);
    if(node->children[1]->type == ls_node_type_expression) {
      list_var = ls_var_list_get_var_by_name(&obj_ptr->object_vars, 
        node->children[1]->token.value.s);
      if(list_var) {
        ls_var_set_reference_value(&ret, list_var);
      }
    }
  }
  return ret;
}

void ls_exec_return_statement(struct ls_exec_t *exec, struct ls_node_t *node) {
  if(!exec->return_var) {
    // error ==> no var given to write to ==> not a function
  } else {
    // write to the var pointer ==> needs to be an empty variable
    *exec->return_var = ls_exec_expr_stack(exec, node->children[0]);
  }
}

ls_boolean_t ls_exec_if_statement(struct ls_exec_t *exec, struct ls_node_t *node) {
  struct ls_var_t res = ls_exec_expr_stack(exec, node->children[0]);
  ls_boolean_t ret = ls_boolean_false;
  if(res.type == ls_var_type_boolean && ls_var_get_ls_boolean_t_value(&res)) {
    ret = ls_boolean_true;
  }
  ls_var_delete(&res);
  return ret;
}

void ls_exec_run(struct ls_exec_t *exec) {
  struct ls_node_t *node = exec->root_node, *pop_node = NULL;
  struct ls_var_t ret_var;
  struct ls_node_stack_t node_stack;
  
  ls_boolean_t if_passed = ls_boolean_false;

  // needs to be max stack size, but for now put 100 ==> ugly python style
  ls_node_stack_create(&node_stack, 100);

  while(node || node_stack.count) {
    while(node) {
      switch(node->type) {
        case ls_node_type_include_statement:
          ls_exec_include_statement(exec, node);
          break;
        case ls_node_type_expression:
          ret_var = ls_exec_expr_stack(exec, node);
          ls_var_delete(&ret_var);
          break;
        case ls_node_type_assign_statement:
          //ls_exec_assign_statement(exec, node);
          break;
        case ls_node_type_function_definition:
          ls_exec_function_definition_statement(exec, node);
          break;
        case ls_node_type_object_definition:
          ls_exec_object_definition_statement(exec, node);
          break;
        case ls_node_type_if_statement:
          if(ls_exec_if_statement(exec, node)) {
            ls_node_stack_push(&node_stack, node);
            node = node->children[1];
            if_passed = ls_boolean_true;
            continue;
          } else {
            if_passed = ls_boolean_false;
          }
          break;
        case ls_node_type_else_statement:
          if(!if_passed) {
            ls_node_stack_push(&node_stack, node);
            node = node->children[0];
            continue;
          }
          if_passed = ls_boolean_false;
          break;
        case ls_node_type_while_statement:
          if(ls_exec_if_statement(exec, node)) {
            ls_node_stack_push(&node_stack, node);
            node = node->children[1];
          } else {
            node = node->next;
          }
          continue;
          break;
        case ls_node_type_function_call:
          ret_var = ls_exec_function_call_statement(exec, node);
          ls_var_delete(&ret_var);
          break;
        case ls_node_type_return_statement:
          ls_exec_return_statement(exec, node);
          break;
        default:
          break;
      }
      node = node->next;
    }
    node = ls_node_stack_pop(&node_stack);
    if(node) {
      if(node->type == ls_node_type_if_statement) {
        node = node->next;
      } else if(node->type == ls_node_type_else_statement) {
        node = node->next;
      }
    }
  }

  ls_node_stack_delete(&node_stack);
}

void ls_exec_delete(struct ls_exec_t *exec) {

}