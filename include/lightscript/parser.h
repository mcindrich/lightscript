#ifndef __LIGHTSCRIPT_PARSER_H__
#define __LIGHTSCRIPT_PARSER_H__

#include <lightscript/lexer.h>
#include <lightscript/node-stack.h>

struct ls_parser_t {
  struct ls_node_t *root_node;
  struct ls_node_t **current_node;
  struct ls_node_t **next_node;
  struct ls_token_t* tokens;
  struct ls_node_stack_t node_stack;
  size_t token_count;
};

void ls_parser_create(struct ls_parser_t *, struct ls_lexer_t *, 
  struct ls_error_t *);
void ls_parser_delete(struct ls_parser_t *);

#endif