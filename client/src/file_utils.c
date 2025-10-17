#include "../include/file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_directory(){
    // Array with all folders to create
    const char *dirs[] = {
        "../files",
        "../files/music",
        "../files/pictures",
        "../files/texts",
        "../files/videos",
        "../files/media",
        "../files/archives",
        "../files/code",
        "../files/others"
    };

    // Number of folders
    size_t n_dirs = sizeof(dirs) / sizeof(dirs[0]);

    // Create each folder
    for (size_t i = 0; i < n_dirs; i++) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "mkdir -p %s", dirs[i]);
        system(cmd);
    }
}

const char* get_directory_by_extension(const char *ext){
    // Audio files
    if (strcmp(ext, "mp3") == 0 || strcmp(ext, "wav") == 0 || strcmp(ext, "flac") == 0)
        return "files/music";

    // Image files
    if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0 || strcmp(ext, "png") == 0 ||
        strcmp(ext, "gif") == 0 || strcmp(ext, "bmp") == 0 || strcmp(ext, "webp") == 0)
        return "files/pictures";

    // Text / document files
    if (strcmp(ext, "txt") == 0 || strcmp(ext, "md") == 0 || strcmp(ext, "csv") == 0 ||
        strcmp(ext, "log") == 0)
        return "files/texts";

    // Video files
    if (strcmp(ext, "mp4") == 0 || strcmp(ext, "mkv") == 0 || strcmp(ext, "avi") == 0 ||
        strcmp(ext, "mov") == 0 || strcmp(ext, "wmv") == 0 || strcmp(ext, "flv") == 0)
        return "files/videos";

    // Audio/Video streaming formats
    if (strcmp(ext, "webm") == 0 || strcmp(ext, "ogg") == 0)
        return "files/media";

    // Compressed / archive files
    if (strcmp(ext, "zip") == 0 || strcmp(ext, "rar") == 0 || strcmp(ext, "7z") == 0 ||
        strcmp(ext, "tar") == 0 || strcmp(ext, "gz") == 0 || strcmp(ext, "bz2") == 0)
        return "files/archives";

    // Code / scripts
    if (strcmp(ext, "c") == 0 || strcmp(ext, "cpp") == 0 || strcmp(ext, "h") == 0 ||
        strcmp(ext, "py") == 0 || strcmp(ext, "js") == 0 || strcmp(ext, "java") == 0)
        return "files/code";

    // Default: unknown files
    return "files/others";
}
