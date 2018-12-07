#include <lightscript/ast.h>
#include <stdlib.h>

void ls_ast_create(struct ls_ast_t *ast, char *code, struct ls_error_t *err) {
  size_t i;

  ls_lexer_create(&ast->lexer, code, err);
  if(err->message) {
    for(i = 0; i < ast->lexer.token_count; i++) {
      if(!LS_IS_CHAR_TOKEN(ast->lexer.tokens[i].type)) {
        free(ast->lexer.tokens[i].value.s);
      }
    }
    return; // when an error occurs, don't parse, just free and quit
  }
  ls_parser_create(&ast->parser, &ast->lexer, err);
  if(err->message) {
    // err
  } else {
    ast->root = ast->parser.root_node;
  }
}

void ls_ast_delete(struct ls_ast_t *ast) {
  ls_lexer_delete(&ast->lexer);
  ls_parser_delete(&ast->parser);
}