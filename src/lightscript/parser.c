#include <lightscript/parser.h>
#include <lightscript/token-stack.h>
#include <stdio.h>
#include <stdlib.h>

static void ls_parser_debug_print_tree(struct ls_node_t *node, int a) {
  if(!node) return;

  for(int i = 0; i < a; i++) printf("  ");
  if(LS_IS_CHAR_TOKEN(node->token.type)) {
    printf("node='%c', type=%d\n", node->token.value.c, node->type);
  } else {
    printf("node='%s', type=%d\n", node->token.value.s, node->type);
  }
  
  for(int i = 0; i < node->children_count; i++) {
    for(int j = 0; j < a; j++) printf("  ");
    printf("child: \n");
    ls_parser_debug_print_tree(node->children[i], a+1);
  }
  ls_parser_debug_print_tree(node->next, a);
}

static void ls_parse_include_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  if(*pos < parser->token_count-1 && 
    parser->tokens[*pos+1].type == ls_token_type_word) {
    //printf("Include: %s\n", parser->tokens[*pos+1].value.s);
    ls_node_create(parser->current_node, 0, ls_node_type_include_statement, 
      &parser->tokens[*pos+1]);
    parser->next_node = &(*parser->current_node)->next;
    ls_token_delete(&parser->tokens[*pos]); // free the keyword token
    *pos += 2;
  } else {
    // err
  }
}

static void ls_parse_def_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_t **itr_node = NULL;
  size_t child_count = 0, pos_init_size = *pos;
  ls_token_delete(&parser->tokens[*pos]);
  ls_node_create(parser->current_node, 2, ls_node_type_function_definition, 
    &parser->tokens[*pos+1]);
  (*parser->current_node)->children[0] = NULL;
  (*parser->current_node)->children[1] = NULL;

  if(parser->tokens[*pos+2].type == ls_token_type_lparenth) {
    pos_init_size = *pos + 3;
    if(parser->tokens[pos_init_size].type == ls_token_type_word) {
      // ==> TODO ==> fix functions with no arguments
      for(*pos += 3; *pos < parser->token_count; *pos += 2) {
        if(parser->tokens[*pos].type == ls_token_type_word) {
          child_count++;
          if(parser->tokens[*pos+1].type != ls_token_type_comma && 
            parser->tokens[*pos+1].type != ls_token_type_rparenth) {
            // err
          } else {
            if(parser->tokens[*pos+1].type == ls_token_type_rparenth) {
              break;
            }
          }
        } else {
          // err
        }
      }
      // printf("CHILD: %d\n", child_count);
      // if it passes the test ==> normal syntax, go normally along
      itr_node = &(*parser->current_node)->children[0];

      ls_node_stack_push(&parser->node_stack, *parser->current_node);
      parser->next_node = &(*parser->current_node)->children[1];

      if(child_count) {
        for(; pos_init_size < parser->token_count; 
          pos_init_size += 2) {
          if(parser->tokens[pos_init_size].type == ls_token_type_word) {
            ls_node_create(itr_node, 0, ls_node_type_function_argument, 
              &parser->tokens[pos_init_size]);
            itr_node = &(*itr_node)->next;
            if(parser->tokens[pos_init_size+1].type != ls_token_type_comma && 
              parser->tokens[pos_init_size+1].type != ls_token_type_rparenth) {
              // err
            } else {
              if(parser->tokens[pos_init_size+1].type == ls_token_type_rparenth) {
                break;
              }
            }
          } else {
            // err
          }
        }
      }
      // update until 'do' token
      *pos += 2;
      ls_token_delete(&parser->tokens[*pos]);
    } else {
      ls_node_stack_push(&parser->node_stack, *parser->current_node);
      parser->next_node = &(*parser->current_node)->children[1];
      *pos += 4;
      ls_token_delete(&parser->tokens[*pos]);
    }
  } else {
    // err
  }
}

static void parser_debug_print_tokens(struct ls_parser_t *parser) {
  for(size_t i = 0; i < parser->token_count; i++) {
    printf("TOKEN %zu: '", i+1);
    if(LS_IS_CHAR_TOKEN(parser->tokens[i].type)) {
      printf("%c", parser->tokens[i].value.c);
    } else {
      printf("%s", parser->tokens[i].value.s);
    }
    printf("', type=%d", parser->tokens[i].type);
    printf(", line=%zu\n", parser->tokens[i].line_number);
  }
}

struct ls_node_t *ls_parse_expression(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_stack_t expr_stack, second_stack;
  struct ls_token_stack_t op_stack;
  struct ls_node_t *tnode0 = NULL, *tnode1 = NULL, *tnode2 = NULL, 
    *pop_node = NULL, *current_child = NULL;
  struct ls_token_t temp_token;
  size_t mss = 0, op_mss = 0, i = *pos, args_count;

  for(; *pos < parser->token_count &&
    LS_IS_VALID_EXPR_TOKEN(parser->tokens[*pos].type); (*pos) += 1, ++mss) {
    //if(LS_IS_OPERATOR(parser->tokens[*pos].type)) op_mss++;
  }
  // allocate with max num of ops ==> just put mss ==> fix the bug later ==>
  // parenthesis and braces should be set to operators ==> LS_IS_OPERATOR func
  ls_token_stack_create(&op_stack, mss);
  // allocate with max size
  ls_node_stack_create(&expr_stack, mss);
  // second stack for functions and arrays so no recursion is called ==> faster
  ls_node_stack_create(&second_stack, mss);

  for(; i < parser->token_count && 
    (LS_IS_VALID_EXPR_TOKEN(parser->tokens[i].type) || second_stack.count); 
      ++i) {
    //printf("TOK: %d\n", parser->tokens[i].type);
    if(parser->tokens[i].type == ls_token_type_comma) {
      while(ls_token_stack_top(&op_stack) && 
        ls_token_stack_top(&op_stack)->type >= parser->tokens[i].type) {
        temp_token = ls_token_stack_pop(&op_stack);
        tnode0 = ls_node_stack_pop(&expr_stack);
        tnode1 = ls_node_stack_pop(&expr_stack);

        //printf("New Node: %c: %s %s\n", temp_token.value.c, tnode0->token.value.s, tnode1->token.value.s);
        ls_node_create(&tnode2, 2, ls_node_type_expression, &temp_token);
        tnode2->children[0] = tnode1;
        tnode2->children[1] = tnode0;
        ls_node_stack_push(&expr_stack, tnode2);
      }
      ls_token_stack_push(&op_stack, &parser->tokens[i]);
    } else if(parser->tokens[i].type == ls_token_type_lparenth || 
      parser->tokens[i].type == ls_token_type_lbracket) {
      // printf("Pushed lparenth\n");
      ls_token_stack_push(&op_stack, &parser->tokens[i]);
    } else if(parser->tokens[i].type == ls_token_type_number || 
      parser->tokens[i].type == ls_token_type_string) {
      ls_node_create(&tnode0, 0, ls_node_type_expression, &parser->tokens[i]);
      ls_node_stack_push(&expr_stack, tnode0);
      // printf("Pushed %s\n", parser->tokens[i].value.s);
    } else if(parser->tokens[i].type == ls_token_type_word) {
      // can be a function or an array, or simply a variable
      if(parser->tokens[i+1].type == ls_token_type_lparenth) {
        // create function node and push it to the second stack
        ls_node_create(&tnode0, 1, ls_node_type_function_call, 
          &parser->tokens[i]);
        ls_node_stack_push(&second_stack, tnode0);
        if(parser->tokens[i+2].type == ls_token_type_rparenth) {
          args_count = 0;
        } else {
          args_count = 1;
        }
      } else if(parser->tokens[i+1].type == ls_token_type_lbracket) {
        // array node
        ls_node_create(&tnode0, 1, ls_node_type_array_element, 
          &parser->tokens[i]);
        ls_node_stack_push(&second_stack, tnode0);
        if(parser->tokens[i+2].type == ls_token_type_rbracket) {
          // error ==> empty array node
        } else {
          args_count = 1;
        }
      } else {
        ls_node_create(&tnode0, 0, ls_node_type_expression, &parser->tokens[i]);
        ls_node_stack_push(&expr_stack, tnode0);
      }
    } else if(LS_IS_OPERATOR(parser->tokens[i].type)) {
      while(ls_token_stack_top(&op_stack) && 
        ls_token_stack_top(&op_stack)->type > parser->tokens[i].type) {
        temp_token = ls_token_stack_pop(&op_stack);
        tnode0 = ls_node_stack_pop(&expr_stack);
        tnode1 = ls_node_stack_pop(&expr_stack);

        //printf("New Node: %c: %s %s\n", temp_token.value.c, tnode0->token.value.s, tnode1->token.value.s);
        ls_node_create(&tnode2, 2, ls_node_type_expression, &temp_token);
        tnode2->children[0] = tnode1;
        tnode2->children[1] = tnode0;
        ls_node_stack_push(&expr_stack, tnode2);
      }
      ls_token_stack_push(&op_stack, &parser->tokens[i]);
      // printf("Push '%c'\n", parser->tokens[i].value.c);
    } else if(parser->tokens[i].type == ls_token_type_rparenth) {
      while(ls_token_stack_top(&op_stack) && ls_token_stack_top(&op_stack)->type != ls_token_type_lparenth) {
        temp_token = ls_token_stack_pop(&op_stack);
        tnode0 = ls_node_stack_pop(&expr_stack);
        tnode1 = ls_node_stack_pop(&expr_stack);

        //printf("New Node: %c: %s %s\n", temp_token.value.c, tnode0->token.value.s, tnode1->token.value.s);
        ls_node_create(&tnode2, 2, ls_node_type_expression, &temp_token);
        tnode2->children[0] = tnode1;
        tnode2->children[1] = tnode0;
        ls_node_stack_push(&expr_stack, tnode2);
      }
      if(second_stack.count) {
        // it's a function
        pop_node = ls_node_stack_pop(&second_stack);
        if(args_count) {
          pop_node->children[0] = ls_node_stack_pop(&expr_stack);
        }
        ls_node_stack_push(&expr_stack, pop_node);
      }
      ls_token_stack_pop(&op_stack);
    } else if(parser->tokens[i].type == ls_token_type_rbracket) {
      while(ls_token_stack_top(&op_stack) && ls_token_stack_top(&op_stack)->type != ls_token_type_lbracket) {
        temp_token = ls_token_stack_pop(&op_stack);
        tnode0 = ls_node_stack_pop(&expr_stack);
        tnode1 = ls_node_stack_pop(&expr_stack);

        //printf("New Node: %c: %s %s\n", temp_token.value.c, tnode0->token.value.s, tnode1->token.value.s);
        ls_node_create(&tnode2, 2, ls_node_type_expression, &temp_token);
        tnode2->children[0] = tnode1;
        tnode2->children[1] = tnode0;
        ls_node_stack_push(&expr_stack, tnode2);
      }
      if(second_stack.count) {
        // it's an array element node
        pop_node = ls_node_stack_pop(&second_stack);
        if(args_count) {
          pop_node->children[0] = ls_node_stack_pop(&expr_stack);
        }
        ls_node_stack_push(&expr_stack, pop_node);
      }
      ls_token_stack_pop(&op_stack);
    }
  }

  while(ls_token_stack_top(&op_stack) &&
    ls_token_stack_top(&op_stack)->type > parser->tokens[i].type) {
    temp_token = ls_token_stack_pop(&op_stack);
    tnode0 = ls_node_stack_pop(&expr_stack);
    tnode1 = ls_node_stack_pop(&expr_stack);

    //printf("New Node: %c: %s %s\n", temp_token.value.c, tnode0->token.value.s, tnode1->token.value.s);
    ls_node_create(&tnode2, 2, ls_node_type_expression, &temp_token);
    tnode2->children[0] = tnode1;
    tnode2->children[1] = tnode0;
    ls_node_stack_push(&expr_stack, tnode2);
  }

  struct ls_node_t *root_node = ls_node_stack_pop(&expr_stack);
  //ls_parser_debug_print_tree(root_node, 0);
  //ls_node_delete(&root_node);

  ls_token_stack_delete(&op_stack);
  ls_node_stack_delete(&expr_stack);
  ls_node_stack_delete(&second_stack);
  return root_node;
}

static void ls_parse_end_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_t *pop_node = NULL;
  if(parser->node_stack.count) {
    // not empty ==> pop and set to current node
    pop_node = ls_node_stack_pop(&parser->node_stack);
    parser->next_node = &pop_node->next;
    ls_token_delete(&parser->tokens[*pos]);
  } else {
    // error ==> nothing to pop ==> probably impossible to happen
    // because it was checked in the lexer but leave just in case
  }
}

static void ls_parse_else_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_t *pop_node = NULL;
  if(parser->node_stack.count) {
    // not empty ==> pop and set to current node
    pop_node = ls_node_stack_pop(&parser->node_stack);
    if(pop_node && pop_node->type == ls_node_type_if_statement) {
      //printf("Valid\n");
      parser->current_node = &pop_node->next;
      ls_node_create(parser->current_node, 1, ls_node_type_else_statement, 
        &parser->tokens[*pos]);
      (*parser->current_node)->children[0] = NULL;
      parser->next_node = &(*parser->current_node)->children[0];
      ls_node_stack_push(&parser->node_stack, *parser->current_node);
    } else {
      // err
    }
    //parser->next_node = &pop_node->next;
  } else {
    // err
  }
}

static void ls_parse_if_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_t *expr_node = NULL;
  ls_node_create(parser->current_node, 2, ls_node_type_if_statement, 
      &parser->tokens[*pos]);

  (*parser->current_node)->children[1] = NULL;

  *pos += 1;
  expr_node = ls_parse_expression(parser, pos, err);
  (*parser->current_node)->children[0] = expr_node;

  if(parser->tokens[*pos].type == ls_token_type_do_keyword) {
    // ok
    ls_token_delete(&parser->tokens[*pos]);
  } else {
    // error
  }
  parser->next_node = &(*parser->current_node)->children[1];
  ls_node_stack_push(&parser->node_stack, *parser->current_node);
}

static void ls_parse_while_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_t *expr_node = NULL;
  ls_node_create(parser->current_node, 2, ls_node_type_while_statement, 
      &parser->tokens[*pos]);
  (*parser->current_node)->children[0] = NULL;
  (*parser->current_node)->children[1] = NULL;

  *pos += 1;
  expr_node = ls_parse_expression(parser, pos, err);
  (*parser->current_node)->children[0] = expr_node;

  if(parser->tokens[*pos].type == ls_token_type_do_keyword) {
    // ok
    ls_token_delete(&parser->tokens[*pos]);
  } else {
    // error
  }
  parser->next_node = &(*parser->current_node)->children[1];
  ls_node_stack_push(&parser->node_stack, *parser->current_node);
}

static void ls_parse_for_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  // printf("FOR\n");
  struct ls_node_t *expr_node = NULL;
  ls_node_create(parser->current_node, 3, ls_node_type_for_statement, 
      &parser->tokens[*pos]);

  (*parser->current_node)->children[0] = NULL;
  (*parser->current_node)->children[1] = NULL;
  (*parser->current_node)->children[2] = NULL;

  *pos += 1;
  expr_node = ls_parse_expression(parser, pos, err);
  (*parser->current_node)->children[0] = expr_node;
  
  if(parser->tokens[*pos].type == ls_token_type_in_keyword) {
    // ok
    ls_token_delete(&parser->tokens[*pos]);
  } else {
    // error
  }

  *pos += 1;
  expr_node = ls_parse_expression(parser, pos, err);
  (*parser->current_node)->children[1] = expr_node;
  // ls_parser_debug_print_tree(expr_node, 0);

  if(parser->tokens[*pos].type == ls_token_type_do_keyword) {
    // ok
    ls_token_delete(&parser->tokens[*pos]);
  } else {
    // error
  }

  parser->next_node = &(*parser->current_node)->children[2];
  ls_node_stack_push(&parser->node_stack, *parser->current_node);
}

static void ls_parse_object_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  ls_token_delete(&parser->tokens[*pos]);
  ls_node_create(parser->current_node, 1, ls_node_type_object_definition, 
    &parser->tokens[*pos+1]);
  (*parser->current_node)->children[0] = NULL;
  parser->next_node = &(*parser->current_node)->children[0];
  ls_node_stack_push(&parser->node_stack, *parser->current_node);
}

static void ls_parse_assign_operator(struct ls_parser_t *parser, size_t last, 
  size_t *pos, struct ls_error_t *err) {
  struct ls_node_t *left = NULL, *right = NULL;
  ls_node_create(parser->current_node, 2, ls_node_type_assign_statement, 
    &parser->tokens[*pos]);
  (*parser->current_node)->children[0] = NULL;
  (*parser->current_node)->children[1] = NULL;

  *pos += 1;

  left = ls_parse_expression(parser, &last, err);
  right = ls_parse_expression(parser, pos, err);

  (*parser->current_node)->children[0] = left;
  (*parser->current_node)->children[1] = right;
  // ls_parser_debug_print_tree(*parser->current_node, 0);
  parser->next_node = &(*parser->current_node)->next;
}

static void ls_parse_return_keyword(struct ls_parser_t *parser, size_t *pos, 
  struct ls_error_t *err) {
  struct ls_node_t *expr = NULL;
  ls_node_create(parser->current_node, 1, ls_node_type_return_statement, 
    &parser->tokens[*pos]);
  (*parser->current_node)->children[0] = NULL;

  *pos += 1;
  expr = ls_parse_expression(parser, pos, err);

  (*parser->current_node)->children[0] = expr;
  parser->next_node = &(*parser->current_node)->next;
}

static void ls_parse_expressions(struct ls_parser_t *parser, size_t last, 
  struct ls_error_t *err) {
  struct ls_node_t *expr = ls_parse_expression(parser, &last, err);
  if(expr) {
    *parser->current_node = expr;
    parser->next_node = &(*parser->current_node)->next;
  } else {
    // err
  }
}

static void ls_parse_func_noreturn(struct ls_parser_t *parser, size_t last, 
  size_t *pos, struct ls_error_t *err) {
  struct ls_node_t *expr = ls_parse_expression(parser, &last, err);
  if(expr) {
    *parser->current_node = expr;
    parser->next_node = &(*parser->current_node)->next;
  } else {
    // err
  }
}

void ls_parser_create(struct ls_parser_t *parser, struct ls_lexer_t *lex, 
  struct ls_error_t *err) {
  struct ls_node_t *backup_root = NULL;
  parser->tokens = lex->tokens;
  parser->token_count = lex->token_count;
  parser->root_node = NULL;
  parser->current_node = &parser->root_node;
  parser->next_node = &parser->root_node;
  ls_node_stack_create(&parser->node_stack, lex->mss); // alloc the stack
  // parser_debug_print_tokens(parser);
  size_t i = 0, last_tok = 0;
  // struct ls_node_t *expr = ls_parse_expression(parser, &i, err);
  // ls_parser_debug_print_tree(expr, 0);
  // ls_node_delete(&expr);
  /*
    TODO ==> free unneccessarry tokens ==> end, struct, do, etc. ==> done!
    TODO ==> error checking and freeing
  */
  for(size_t i = 0; i < parser->token_count; i++) {
    if(parser->tokens[i].type == ls_token_type_include_keyword) {
      ls_parse_include_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_def_keyword) {
      ls_parse_def_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_end_keyword) {
      ls_parse_end_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_if_keyword) {
      ls_parse_if_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_else_keyword) {
      ls_parse_else_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_for_keyword) {
      ls_parse_for_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_while_keyword) {
      ls_parse_while_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_return_keyword) {
      ls_parse_return_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_object_keyword) {
      ls_parse_object_keyword(parser, &i, err);
      last_tok = i + 1;
    } else if(parser->tokens[i].type == ls_token_type_semicolon) {
      ls_parse_expressions(parser, last_tok, err);
      last_tok = i + 1;
    }
    parser->current_node = parser->next_node;
  }

  ls_node_stack_delete(&parser->node_stack);
#ifdef PARSER_DEBUGGING
  ls_parser_debug_print_tree(parser->root_node, 0);
#endif
}

void ls_parser_delete(struct ls_parser_t *parser) {
  ls_node_delete(&parser->root_node);
}