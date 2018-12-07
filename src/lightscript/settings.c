#include <lightscript/settings.h>
#include <stdlib.h>

void ls_settings_create(struct ls_settings_t *stg) {
  stg->file_name = NULL;
}

void ls_settings_set_file_name(struct ls_settings_t *stg, char *name) {
  stg->file_name = name;
}

void ls_settings_delete(struct ls_settings_t *stg) {

}