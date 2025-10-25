#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define PORT 8080
#define BACKLOG 10
#define BUF_SIZE 4096

// Function to send a file to the client
void send_file(int client_fd, const char *filepath) {
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        // File not found → 404
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        send(client_fd, not_found, strlen(not_found), 0);
        return;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Build HTTP header
    char header[256];
    int header_len = snprintf(header, sizeof(header),
                              "HTTP/1.1 200 OK\r\n"
                              "Content-Length: %ld\r\n"
                              "Connection: close\r\n\r\n",
                              file_size);
    send(client_fd, header, header_len, 0);

    // Send file in chunks
    char buffer[BUF_SIZE];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_fd, buffer, n, 0);
    }

    fclose(fp);
}

int main() {
    signal(SIGPIPE, SIG_IGN);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("error building socket");
        exit(EXIT_FAILURE);
    }

    // Allow port reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("HTTP server listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // Generate updated index.html files
        system("python3 generate_indexes.py");

        // Receive HTTP request
        char buffer[BUF_SIZE];
        ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            close(client_fd);
            continue;
        }
        buffer[bytes] = '\0';

        // Extract HTTP method and path
        char method[16], path[256];
        sscanf(buffer, "%s %s", method, path);

        // Print client request info in terminal
        printf("[CLIENT] %s:%d — %s %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               method, path);

        // if (strcmp(method, "GET") != 0) {
        //     // Only GET method is supported
        //     const char *not_allowed = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        //     send(client_fd, not_allowed, strlen(not_allowed), 0);
        //     close(client_fd);
        //     continue;
        // }

        // Build real file path
        char filepath[512];
        snprintf(filepath, sizeof(filepath), "../files%s", path);

        // If URL ends with '/', serve index.html if available
        struct stat st;
        if (stat(filepath, &st) == 0 && S_ISDIR(st.st_mode)) {
            strncat(filepath, "/index.html", sizeof(filepath) - strlen(filepath) - 1);
        }

        send_file(client_fd, filepath);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
