#ifndef URL_H
#define URL_H

int parse_url(const char *url, char **host, char **port, char **path);
char *get_filename_from_path(const char *path);
char *get_extension_from_filename(const char *filename);

#endif
