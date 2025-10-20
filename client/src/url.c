#include "../include/url.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_url(const char *url, char **host, char **port, char **path){
    // Check if URL starts with http://
    if(strncmp(url, "http://", 7) != 0){
        fprintf(stderr, "Error: URL must start with http://\n");
        return -1;
    }

    // Extract the part after http://
    const char *authority_start = url + 7;
    const char *path_start = strchr(authority_start, '/');
    size_t authority_len;

    // If path exists, extract it
    if(path_start){
        authority_len = path_start - authority_start;
        *path = strdup(path_start); // Copy path
    }else{
        authority_len = strlen(authority_start);
        *path = strdup("/");
    }

    // Extract host and port
    char *authority = strndup(authority_start, authority_len);
    if(!authority){
        perror("strndup failed");
        return -1;
    }

    // Split host and port using ':'
    char *colon = strchr(authority, ':');
    if(colon){
        *colon = '\0'; // Split string at colon
        *host = strdup(authority);
        *port = strdup(colon + 1);
    }else{
        *host = strdup(authority);
        *port = strdup("8080"); // Default HTTP port
    }

    free(authority); // Free temporary buffer
    // Check if all allocations succeeded
    if(!*host || !*port || !*path){
        free(*host);
        free(*port);
        free(*path);
        return -1;
    }

    return 0;
}

char *get_filename_from_path(const char *path){
    // Search for the last '/'
    const char *filename = strrchr(path, '/');
    if (filename && *(filename + 1) != '\0') {
        return strdup(filename + 1); // returns part after the '/'
    }

    // Se não houver nome de arquivo → usar "index.html"
    const char *default_file = "index.html";
    return strdup(default_file);
}

char *get_extension_from_filename(const char *filename){
    // Search for the last '.'
    const char *extension = strrchr(filename, '.');
    if (extension && *(extension + 1) != '\0') {
        return strdup(extension + 1); // returns part after the '.'
    }

    // Otherwise, no extension provided — return nothing
    return strdup("");
}
