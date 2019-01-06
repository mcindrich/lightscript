#include <lightscript/array.h>
#include <stdlib.h>

void ls_array_create(struct ls_array_t *arr, size_t size) {
  if(size) {
    arr->vars = (struct ls_var_t *) malloc(sizeof(struct ls_var_t) * size);
  } else {
    arr->vars = NULL;
  }
  arr->size = size;
}

void ls_array_set_element(struct ls_array_t *arr, size_t pos, 
  struct ls_var_t *var) {
  size_t i;
  if(pos > arr->size) {
    // reallocate and add little extra space ==> for example using for loop to create an array
    // --> allocate more at once
    arr->vars = (struct ls_var_t *) realloc(arr->vars, sizeof(struct ls_var_t) *
      (pos + ARRAY_EXTRA_ALLOC_SPACE));
    for(i = arr->size; i < pos + ARRAY_EXTRA_ALLOC_SPACE; i++) {
      ls_var_create(&arr->vars[i]);
    }
    arr->size = pos + ARRAY_EXTRA_ALLOC_SPACE;
  }
  
  arr->vars[pos] = *var;
}

void ls_array_delete(struct ls_array_t *arr) {
  size_t i = 0;
  if(arr->size) {
    for(i = 0; i < arr->size; i++) {
      ls_var_delete(&arr->vars[i]);
    }
    free(arr->vars);
  }
}