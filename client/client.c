#include <sys/socket.h> // socket functions (socket, connect, send, recv...)
#include <sys/types.h> // basic types (necessary on some platforms)
#include <netinet/in.h> // struct sockaddr_in and constants for IPv4 family
#include <netdb.h> // gethostbyname / getaddrinfo
#include <stdio.h> // I/O (printf, fopen, fputs...)
#include <string.h> // string manipulation and memset
#include <stdlib.h> // malloc, free, exit, atoi...
#include <unistd.h> // close, read, write, fork...
#include <errno.h> // errno, perror
#include <arpa/inet.h>// inet_pton, inet_ntop

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

char *get_filename_from_path(const char *path) {
    // Search for the last '/'
    const char *filename = strrchr(path, '/');
    if (filename && *(filename + 1) != '\0') {
        return strdup(filename + 1); // returns part after the '/'
    } else {
        return strdup("index.html"); // default if no name exists
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("\n Usage: %s <URL> \n", argv[0]);
        return 1;
    }

    char *url = argv[1];
    char *host = NULL, *port = NULL, *path = NULL;

    // Parse the URL
    if(parse_url(url, &host, &port, &path) != 0){
        fprintf(stderr, "Failed to parse URL\n");
        return 1;
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    // Perform DNS lookup
    int status = getaddrinfo(host, port, &hints, &res);
    if(status != 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        goto cleanup;
    }

    // Create socket and connect
    int sockfd = -1;
    struct addrinfo *p;
    for(p = res; p != NULL; p = p->ai_next){
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sockfd == -1) continue;
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            continue;
        }
        break; // Connection succeeded
    }
    if(!p){
        fprintf(stderr, "Failed to connect to any address\n");
        freeaddrinfo(res);
        goto cleanup;
    }
    freeaddrinfo(res); // Free DNS results

    // Construct HTTP GET request
    char request[1024];
    int req_len = snprintf( request, sizeof(request),
                            "GET %s HTTP/1.1\r\n"
                            "Host: %s\r\n"
                            "Connection: close\r\n\r\n",
                            path, host);

    if(req_len < 0 || (size_t)req_len >= sizeof(request)){
        fprintf(stderr, "Request too long\n");
        close(sockfd);
        goto cleanup;
    }

    // Send request
    if(send(sockfd, request, req_len, 0) == -1){
        perror("send failed");
        close(sockfd);
        goto cleanup;
    }

    // Create "files" directory (ignore error if it already exists)
    system("mkdir -p files");

    // Extract the file name from the path
    char *filename = get_filename_from_path(path);

    // Construct the final path: files/<name>
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "files/%s", filename);

    // Open file
    FILE *fp = fopen(filepath, "wb");
    if (!fp) {
        perror("Error creating output file");
        close(sockfd);
        goto cleanup;
    }

    // Receive HTTP response data
    char buffer[4096];
    ssize_t bytes_received;
    int header_skipped = 0;
    char *body_start;

    while((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0){
        // Skip HTTP header (everything before "\r\n\r\n")
        if (!header_skipped) {
            body_start = strstr(buffer, "\r\n\r\n");
            if (body_start) {
                body_start += 4;
                fwrite(body_start, 1, bytes_received - (body_start - buffer), fp);
                header_skipped = 1;
            }
        } else {
            fwrite(buffer, 1, bytes_received, fp);
        }
    }
    if(bytes_received == -1){
        perror("recv failed");
    }

    printf("File saved to: %s\n", filepath);

    fclose(fp);
    close(sockfd);
    free(filename);

    // Free memory from parse_url
    cleanup:
        free(host);
        free(port);
        free(path);

    return EXIT_SUCCESS;
}
