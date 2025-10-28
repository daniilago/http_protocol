#include <string.h>
#include "mime.h"

const char* get_content_type(const char* path) {
    if (strstr(path, ".mp3")) return "audio/mp3";
    if (strstr(path, ".wav")) return "audio/wav";
    if (strstr(path, ".flac")) return "audio/flac";

    if (strstr(path, ".jpg") || strstr(path, ".jpeg")) return "image/jpeg";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".gif")) return "image/gif";
    if (strstr(path, ".bmp")) return "image/bmp";
    if (strstr(path, ".webp")) return "image/webp";

    if (strstr(path, ".txt")) return "text/plain";
    if (strstr(path, ".md")) return "text/markdown";
    if (strstr(path, ".csv")) return "text/csv";

    if (strstr(path, ".mp4")) return "video/mp4";
    if (strstr(path, ".mkv")) return "video/x-matroska";
    if (strstr(path, ".webm")) return "video/webm";
    if (strstr(path, ".avi")) return "video/x-msvideo";
    if (strstr(path, ".mov")) return "video/quicktime";

    if (strstr(path, ".zip")) return "application/zip";
    if (strstr(path, ".rar")) return "application/vnd.rar";
    if (strstr(path, ".7z")) return "application/x-7z-compressed";
    if (strstr(path, ".tar")) return "application/x-tar";
    if (strstr(path, ".gz")) return "application/gzip";

    if (strstr(path, ".c")) return "text/x-c";
    if (strstr(path, ".cpp")) return "text/x-c++";
    if (strstr(path, ".py")) return "text/x-python";
    if (strstr(path, ".js")) return "application/javascript";
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".css")) return "text/css";

    return "application/octet-stream";
}
