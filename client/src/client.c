#include <sys/socket.h> // socket functions (socket, connect, send, recv...)
#include <sys/types.h> // basic types (necessary on some platforms)
#include <netinet/in.h> // struct sockaddr_in and constants for IPv4 family
#include <netdb.h> // gethostbyname / getaddrinfo
#include <stdio.h> // I/O (printf, fopen, fputs...)
#include <string.h> // string manipulation and memset
#include <stdlib.h> // malloc, free, exit, atoi...
#include <unistd.h> // close, read, write, fork...
#include <arpa/inet.h>// inet_pton, inet_ntop

#include "../include/url.h"
#include "../include/file_utils.h"

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
        free(host); free(port); free(path);
        return 1;
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

    freeaddrinfo(res); // Free DNS results

    if(!p){
        fprintf(stderr, "Failed to connect to any address\n");
        free(host); free(port); free(path);
        return 1;
    }

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
        free(host); free(port); free(path);
        return 1;
    }

    // Send request
    if(send(sockfd, request, req_len, 0) == -1){
        perror("send failed");
        close(sockfd);
        free(host); free(port); free(path);
        return 1;
    }

    // Create "files" directory (ignore if it already exists)
    create_directory();

    // Extract the file name from the path
    char *filename = get_filename_from_path(path);

    // Extract the extension_file from the filename
    char *extension = get_extension_from_filename(filename);

    // Construct the final path: files/<name>
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "../%s/%s", get_directory_by_extension(extension), filename);

    // Receive HTTP response data
    char buffer[4096];
    ssize_t bytes_received;
    int header_skipped = 0;
    char *body_start;

    int save_file = 1;
    FILE *fp = NULL;

    while((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0){
        buffer[bytes_received] = '\0'; // ensure null-terminated string

        // Skip HTTP header (everything before "\r\n\r\n")
        if (!header_skipped) {
            body_start = strstr(buffer, "\r\n\r\n");

            if (body_start) {
                // Separate header and body
                *body_start = '\0';  // temporarily end string at header's end

                printf("\n===== SERVER RESPONSE HEADER =====\n");
                printf("%s", buffer);
                printf("\n===================================\n\n");

                if (strncmp(buffer, "HTTP/1.1 200", 12) == 0) {
                    fp = fopen(filepath, "wb");

                    if (!fp) {
                        perror("Error creating output file");
                        close(sockfd);
                        free(host); free(port); free(path);
                        free(filename); free(extension);
                        return 1;
                    }
                }else{
                    // Not 200 OK → do not save the file, stop reading body
                    save_file = 0;
                    break;
                }

                body_start += 4;

                fwrite(body_start, 1, bytes_received - (body_start - buffer), fp);
                header_skipped = 1;
            }
        } else {
            fwrite(buffer, 1, bytes_received, fp);
        }
    }
    if(bytes_received == -1){
        perror("Recv failed");
    }

    if(save_file) printf("File saved to: %s\n", filepath);


    // Free memory
    if (fp) fclose(fp);
    free(host); free(port); free(path);
    free(filename); free(extension);

    close(sockfd);

    return 0;
}
