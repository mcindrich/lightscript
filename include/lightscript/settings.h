#ifndef __LIGHTSCRIPT_SETTINGS_H__
#define __LIGHTSCRIPT_SETTINGS_H__

// settings for the interpreter

struct ls_settings_t {
  char *file_name;
};

void ls_settings_create(struct ls_settings_t *);
void ls_settings_set_file_name(struct ls_settings_t *, char *);
void ls_settings_delete(struct ls_settings_t *);

#endif