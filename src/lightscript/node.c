#include <lightscript/node.h>
#include <stdlib.h>

void ls_node_create(struct ls_node_t **node, size_t cc, 
	enum ls_node_type_t type, struct ls_token_t *tok) {
  *node = (struct ls_node_t *) malloc(sizeof(struct ls_node_t));
  (*node)->children = NULL;
  (*node)->next = NULL;
  (*node)->children_count = cc;
	if(cc) {
		(*node)->children = calloc(cc, sizeof(struct ls_node_t *));
	}
  (*node)->token = *tok;
	(*node)->type = type;
}

void ls_node_delete(struct ls_node_t **node) {
  if(*node == NULL) return;
	struct ls_node_t *next = (*node)->next, **child = (*node)->children;
	struct ls_token_t *tok = &(*node)->token;
	size_t i = 0;
	const size_t cnt = (*node)->children_count;
	
	ls_node_delete(&next);
	for(; i < cnt; i++) {
		ls_node_delete(&child[i]);
	}
	if(child) {
		free(child);
	}
	if(tok) {
		ls_token_delete(tok);
	}
	free(*node);
}