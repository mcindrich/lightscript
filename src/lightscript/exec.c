#include <lightscript/exec.h>
#include <lightscript/node-stack.h>
#include <lightscript/var-stack.h>
#include <lightscript/defines.h>
#include <lightscript/u8-stack.h>
#include <lightscript/function.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    default:
      printf("OTHER TYPE\n");
      break;
  }
}

static struct ls_var_t ls_exec_expr_stack(struct ls_exec_t *exec,
  struct ls_node_t *node) {
  struct ls_node_stack_t node_stack;
  struct ls_var_stack_t var_stack;
  struct ls_var_t ret_var, temp_var, left ,right;
  enum ls_token_type_t tt;
  struct ls_node_t *current_node = node, *pop_node = NULL;

  ls_var_create(&ret_var);
  ls_var_create(&temp_var);
  
  // needs to be precounted
  ls_node_stack_create(&node_stack, 10);
  ls_var_stack_create(&var_stack, 10);
  
  while(current_node || node_stack.count) {
    while(current_node) {
      ls_node_stack_push(&node_stack, current_node);
      if(current_node->children)
        current_node = current_node->children[0];
      else current_node = NULL;
    }
    pop_node = ls_node_stack_pop(&node_stack);
    tt = pop_node->token.type;
    if(LS_IS_CHAR_TOKEN(pop_node->token.type)) {
      printf("Popped: %c\n", pop_node->token.value.c);
    }

    if(pop_node->type == ls_node_type_expression) {
      if(pop_node->children) {
        if(var_stack.counter == 2) {
          right = ls_var_stack_pop(&var_stack);
          left = ls_var_stack_pop(&var_stack);

          ls_exec_debug_print_var(&left);
          ls_exec_debug_print_var(&right);

          if(tt == ls_token_type_plus) {
            temp_var = ls_var_operator_add(&left, &right);
          } else if(tt == ls_token_type_multiply) {
            temp_var = ls_var_operator_mul(&left, &right);
          } else if(tt == ls_token_type_minus) {
            temp_var = ls_var_operator_sub(&left, &right);
          } else if(tt == ls_token_type_divide) {
            temp_var = ls_var_operator_div(&left, &right);
          }
          ls_var_stack_push(&var_stack, &temp_var);
          ls_var_delete(&left);
          ls_var_delete(&right);
        } else {
          ls_node_stack_push(&node_stack, pop_node);
          current_node = pop_node->children[1];
        }
      } else {
        if(tt == ls_token_type_number) {
          double val = atof(pop_node->token.value.s);
          // check if it's an integer ==> TODO: all int types
          // search for '.' and if it exists ==> it's a double
          char* cont = strchr(pop_node->token.value.s, '.');
          if(!cont) {
            printf("Creating s32 %d\n", (s32)val);
            ls_var_set_s32_value(&temp_var, (s32)val);
          } else {
            ls_var_set_double_value(&temp_var, val);
          }
          ls_var_stack_push(&var_stack, &temp_var);
        } else if(tt == ls_token_type_word) {

        } else if(tt == ls_token_type_string) {

        }
        current_node = NULL;
      }
    } else if(pop_node->type == ls_node_type_function_call) {
      current_node = NULL;
    }
  }

  temp_var = ls_var_stack_pop(&var_stack);
  printf("result: \n");
  ls_exec_debug_print_var(&temp_var);

  ls_node_stack_delete(&node_stack);
  ls_var_stack_delete(&var_stack);
  return ret_var;
}

static struct ls_var_t ls_exec_expr_recursion(struct ls_exec_t *, 
  struct ls_node_t *);
struct ls_var_t ls_exec_function_call_statement(struct ls_exec_t *, 
  struct ls_node_t *);

struct ls_var_t ls_exec_expr_recursion(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_var_t ret, left, right, *list_var;
  ls_var_create(&ret);

  enum ls_token_type_t tt = node->token.type;
  if(node->type == ls_node_type_expression) {
    if(node->children_count) {
      left = ls_exec_expr_recursion(exec, node->children[0]);
      right = ls_exec_expr_recursion(exec, node->children[1]);
      if(tt == ls_token_type_plus) {
        ret = ls_var_operator_add(&left, &right);
      } else if(tt == ls_token_type_minus) {
        ret = ls_var_operator_sub(&left, &right);
      } else if(tt == ls_token_type_multiply) {
        ret = ls_var_operator_mul(&left, &right);
      } else if(tt == ls_token_type_divide) {
        ret = ls_var_operator_div(&left, &right);
      } else if(tt == ls_token_type_percent) {
        ret = ls_var_operator_mod(&left, &right);
      }  else if(tt == ls_token_type_less_than) {
        ret = ls_var_operator_lt(&left, &right);
      } else if(tt == ls_token_type_less_equal) {
        ret = ls_var_operator_le(&left, &right);
      } else if(tt == ls_token_type_greater_than) {
        ret = ls_var_operator_gt(&left, &right);
      } else if(tt == ls_token_type_greater_equal) {
        ret = ls_var_operator_ge(&left, &right);
      } else if(tt == ls_token_type_equal_equal) {
        ret = ls_var_operator_eq(&left, &right);
      } else if(tt == ls_token_type_not_equal) {
        ret = ls_var_operator_ne(&left, &right);
      } else if(tt == ls_token_type_and_op) {
        ret = ls_var_operator_and(&left, &right);
      } else if(tt == ls_token_type_or_op) {
        ret = ls_var_operator_or(&left, &right);
      }
      ls_var_delete(&left);
      ls_var_delete(&right);
    } else {
      if(tt == ls_token_type_number) {
        double val = atof(node->token.value.s);
        // check if it's an integer ==> TODO: all int types
        // search for '.' and if it exists ==> it's a double
        char* cont = strchr(node->token.value.s, '.');
        if(!cont) {
          ls_var_set_s32_value(&ret, (s32)val);
        } else {
          ls_var_set_double_value(&ret, val);
        }
      } else if(tt == ls_token_type_string) {
        ls_var_set_string_value(&ret, node->token.value.s);
      } else if(tt == ls_token_type_word) {
        // find variable or object
        if(exec->local_vars) {
          // get local variable first
          list_var = ls_var_list_get_var_by_name(exec->local_vars, 
            node->token.value.s);
          if(!list_var) {
            // get global variable
            list_var = ls_var_list_get_var_by_name(exec->global_vars, 
              node->token.value.s);
          }
        } else {
          // get global variable
          list_var = ls_var_list_get_var_by_name(exec->global_vars, 
            node->token.value.s);
        }
        if(list_var) {
          ls_var_copy(&ret, list_var);
        } else {
          // error ==> no such variable
        }
      }
    }
  } else if(node->type == ls_node_type_function_call) {
    ret = ls_exec_function_call_statement(exec, node);
    // ls_var_set_s32_value(&ret, 0);
  }
  return ret;
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

struct ls_var_t ls_exec_function_call_statement(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  struct ls_var_t ret_var, temp_var;
  struct ls_var_list_t args;

  ls_var_list_create(&args, 0);
  ls_var_create(&ret_var);

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
          temp_var = ls_exec_expr_recursion(exec, left);
          ls_var_list_add_var(&args, &temp_var);
        }
        temp_var = ls_exec_expr_recursion(exec, right);
        ls_var_list_add_var(&args, &temp_var);
        if(pop_node->children)
          current_node = pop_node->children[1];
        else current_node = NULL;
      }

      ls_node_stack_delete(&node_stack);
    } else {
      // one argument
      temp_var = ls_exec_expr_recursion(exec, node->children[0]);
      ls_var_list_add_var(&args, &temp_var);
    }
  }

  struct ls_var_t *func = ls_var_list_get_var_by_name_and_type(
      exec->global_vars, node->token.value.s, ls_var_type_function);
  
  if(func) {
    struct ls_function_t *func_ptr = ls_var_get_function_value(func);
    ret_var = ls_function_execute(func_ptr, exec->global_vars, &args);
  } else {
    // err => function does not exist
  }
  ls_var_list_delete(&args);
  return ret_var;
}

static void ls_exec_assign_statement(struct ls_exec_t *exec, 
  struct ls_node_t *node) {
  // get left side and check if the variable exists and if it doesn't, create it
  // https://www.geeksforgeeks.org/inorder-tree-traversal-without-recursion/
  // traversing without recursion ==> speed
  
  /*
  struct ls_node_stack_t node_stack;
  struct ls_var_t temp_var;
  struct ls_node_t *current_node = node->children[1]->children[0], *pop_node = NULL, *left = NULL, *right = NULL;
  // needs to be precounted
  ls_node_stack_create(&node_stack, 10);

  u8 cnt = 0;

  // comma counting ==> functions and arrays
  
  while((current_node && current_node->token.type == ls_token_type_comma) || node_stack.counter) {
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
      cnt++;
    }
    cnt++;
    if(pop_node->children)
      current_node = pop_node->children[1];
    else current_node = NULL;
  }

  printf("Cnt: %d\n", cnt);

  ls_node_stack_delete(&node_stack);*/

  /*struct ls_node_stack_t node_stack;
  struct ls_u8_stack_t bool_stack;
  struct ls_var_t temp_var;
  struct ls_node_t *current_node = node->children[1], *pop_node = NULL, *left = NULL, *right = NULL;
  // needs to be precounted
  ls_node_stack_create(&node_stack, 20);
  ls_u8_stack_create(&bool_stack, 20);
  u8 pick = 0;

  while(current_node || node_stack.counter) {
    while(current_node) {
      ls_node_stack_push(&node_stack, current_node);
      if(current_node->children) {
        if(current_node->children[0]->token.type == ls_token_type_number && current_node->children[1]->token.type != ls_token_type_number) {
          pick = 1;
        } else if(current_node->children[1]->token.type == ls_token_type_number && current_node->children[0]->token.type != ls_token_type_number) {
          pick = 0;
        } else if(current_node->children[0]->token.type != ls_token_type_number && current_node->children[1]->token.type != ls_token_type_number) {
          printf("Diff: %d - %d\n", current_node->children[0]->token.type , current_node->children[1]->token.type);
          pick = current_node->children[0]->token.type > current_node->children[1]->token.type;
        }
        current_node = current_node->children[pick];
        ls_u8_stack_push(&bool_stack, pick);
        //printf("Pushed %d\n", pick);
        //printf("Pick = %d\n", pick);
      } else {
        current_node = NULL;
      }
    }
    pop_node = ls_node_stack_pop(&node_stack);
    
    if(LS_IS_CHAR_TOKEN(pop_node->token.type)) {
      printf("Char: %c\n", pop_node->token.value.c);
    } else {
      printf("String: %s\n", pop_node->token.value.s);
    }
    if(pop_node->children) {
      pick = ls_u8_stack_pop(&bool_stack);
      //printf("Popped %d\n", pick);
      if(pick) {
        current_node = pop_node->children[0];
      } else {
        current_node = pop_node->children[1];
      }
    } else {
      current_node = NULL;
    }
  }
  ls_node_stack_delete(&node_stack);*/

  // add support for left and right comma interpreting ==> a, b = 20, 30;
  char *var_name = node->children[0]->token.value.s;
  struct ls_var_t *ret_var = NULL, expr_value;
  ls_var_create(&expr_value);
  // check if a var exists
  if(exec->local_vars) {
    // first locals then global
    ret_var = ls_var_list_get_var_by_name(exec->local_vars, var_name);
    expr_value = ls_exec_expr_recursion(exec, node->children[1]);
    if(!ret_var) {
      ret_var = ls_var_list_get_var_by_name(exec->global_vars, var_name);
      if(!ret_var) {
        // create variable and push it to the list
        ls_var_set_name(&expr_value, var_name);
        ls_var_list_add_var(exec->local_vars, &expr_value);
      } else {
        // there is a global one ==> change that one
        ls_var_delete_value(ret_var);
        ret_var->value = expr_value.value;
        ret_var->type = expr_value.type;
      }
    } else {
      ls_var_delete_value(ret_var);
      ret_var->value = expr_value.value;
      ret_var->type = expr_value.type;
      // change value ==> apply assign operator
    }
  } else {
    // just global
    ret_var = ls_var_list_get_var_by_name(exec->global_vars, var_name);
    expr_value = ls_exec_expr_recursion(exec, node->children[1]);

    if(!ret_var) {
      // create variable and push it to the list
      // ls_exec_debug_print_var(&expr_value);
      ls_var_set_name(&expr_value, var_name);
      ls_var_list_add_var(exec->global_vars, &expr_value);
      //ls_exec_debug_print_var(&expr_value);
      //printf("Value: %s\n", ls_var_get_string_value(&expr_value));
    } else {
      // change value ==> apply assign operator
      ls_var_delete_value(ret_var);
      ret_var->value = expr_value.value;
      ret_var->type = expr_value.type;
    }
  }
  
}

void ls_exec_return_statement(struct ls_exec_t *exec, struct ls_node_t *node) {
  if(!exec->return_var) {
    // error ==> no var given to write to ==> not a function
  } else {
    // write to the var pointer ==> needs to be an empty variable
    *exec->return_var = ls_exec_expr_recursion(exec, node->children[0]);
  }
}

boolean ls_exec_if_statement(struct ls_exec_t *exec, struct ls_node_t *node) {
  struct ls_var_t res = ls_exec_expr_recursion(exec, node->children[0]);
  boolean ret = boolean_false;
  if(res.type == ls_var_type_boolean && ls_var_get_boolean_value(&res)) {
    ret = boolean_true;
  }
  ls_var_delete(&res);
  return ret;
}

void ls_exec_run(struct ls_exec_t *exec) {
  struct ls_node_t *node = exec->root_node, *pop_node = NULL;
  struct ls_var_t ret_var;
  struct ls_node_stack_t node_stack;

  boolean if_passed = boolean_false;

  // needs to be max stack size, but for now put 100 ==> ugly python style
  ls_node_stack_create(&node_stack, 100);

  while(node || node_stack.count) {
    while(node) {
      switch(node->type) {
        case ls_node_type_assign_statement:
          ls_exec_assign_statement(exec, node);
          break;
        case ls_node_type_function_definition:
          ls_exec_function_definition_statement(exec, node);
          break;
        case ls_node_type_if_statement:
          if(ls_exec_if_statement(exec, node)) {
            ls_node_stack_push(&node_stack, node);
            node = node->children[1];
            if_passed = boolean_true;
            continue;
          } else {
            if_passed = boolean_false;
          }
          break;
        case ls_node_type_else_statement:
          if(!if_passed) {
            ls_node_stack_push(&node_stack, node);
            node = node->children[0];
            continue;
          }
          if_passed = boolean_false;
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