#include <lightscript/error.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void ls_error_create(struct ls_error_t *err) {
  err->message = NULL;
  err->line = 0;
}

void ls_error_print(struct ls_error_t *err) {
  fprintf(stderr, ">> error occured: \n");
  if(err->line) { // can be another type of an error ==> stack size wrong, etc.
    fprintf(stderr, "- line: %zu\n", err->line);
  }
  fprintf(stderr, "- message: <%s>\n", err->message);
}

void ls_error_set(struct ls_error_t *err, size_t ln, const char *msg) {
  err->line = ln;
  size_t len = strlen(msg);
  err->message = (char *) malloc(sizeof(char) * (len+1));
  memcpy(err->message, msg, len);
  err->message[len] = 0;
}

void ls_error_delete(struct ls_error_t *err) {
  if(err->message) {
    free(err->message);
  }
}