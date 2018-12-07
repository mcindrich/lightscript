#ifndef __LIGHTSCRIPT_AST_H__
#define __LIGHTSCRIPT_AST_H__

#include <lightscript/parser.h>

struct ls_ast_t {
  struct ls_node_t* root;
  struct ls_parser_t parser;
  struct ls_lexer_t lexer;
};

void ls_ast_create(struct ls_ast_t *, char *, struct ls_error_t *);
void ls_ast_delete(struct ls_ast_t *);

#endif