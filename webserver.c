#include <dirent.h>
#include <netdb.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int response_status = 200;
bool sig_recieved = false;
int MAX_REQUEST_SIZE = 1500;

int server_file_descriptor = -1;

char absolute_path[PATH_MAX];

char* get_requested_resource_path(char* request) {
    int ptr = 0;
    while (request[ptr] != '\n') {
        ptr++;
    }
    char* path = (char*)malloc(sizeof(char) * ptr + 15);
    ptr = 0;
    while (request[ptr] != '/') {
        ptr++;
    }
    ptr++;
    path[0] = '.';
    path[1] = '/';
    int path_ptr = 2;
    while (request[ptr] != ' ') {
        path[path_ptr++] = request[ptr++];
    }
    path[path_ptr] = '\0';

    realpath(path, absolute_path);

    return absolute_path;
}

char* read_dir(const char* path) {
    DIR* dir = opendir(path);

    if (dir == NULL) {
        return NULL;
    }
    char* document_begin =
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>Directory</title></head>"
        "<body>";
    char* document_end =
        "</body>"
        "</html>";

    int len = strlen(document_begin) + strlen(document_end) + 1;
    struct dirent* dp;
    while ((dp = readdir(dir)) != NULL) {
        len += 2 * strlen(dp->d_name) + 38;
    }
    char* page_string = malloc(len + 1);
    int page_ptr = 0;
    dir = opendir(path);
    for (int i = 0; i < strlen(document_begin); i++) {
        page_string[page_ptr++] = document_begin[i];
    }
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_type == DT_DIR) {
            continue;
        }
        char *open_tag = "<p><a href='", *middle_tag = "'>",
             *close_tag = "</a></p>";
        for (int i = 0; i < strlen(open_tag); i++) {
            page_string[page_ptr++] = open_tag[i];
        }
        // inside href
        for (int i = 0; i < strlen(dp->d_name); i++) {
            page_string[page_ptr++] = dp->d_name[i];
        }
        for (int i = 0; i < strlen(middle_tag); i++) {
            page_string[page_ptr++] = middle_tag[i];
        }
        // between the tags >...<
        for (int i = 0; i < strlen(dp->d_name); i++) {
            page_string[page_ptr++] = dp->d_name[i];
        }

        for (int i = 0; i < strlen(close_tag); i++) {
            page_string[page_ptr++] = close_tag[i];
        }
    }
    for (int i = 0; i < strlen(document_end); i++) {
        page_string[page_ptr++] = document_end[i];
    }
    page_string[page_ptr] = '\0';

    return page_string;
}

char* read_file(char* path) {
    char* dir_string = read_dir(path);
    if (dir_string != NULL) {
        return dir_string;
    }

    FILE* fp = fopen(path, "r");

    if (fp == NULL) {
        char* buffer = (char*)malloc(20 * sizeof(char));
        char* not_found_msg = "Page not found";
        response_status = 404;
        int i = 0;
        for (; i < strlen(not_found_msg); i++) {
            buffer[i] = not_found_msg[i];
        }
        buffer[i] = '\0';
        return buffer;
    }
    response_status = 200;
    int buffer_size = 2;
    char* buffer = (char*)malloc(2 * sizeof(char));
    char c;
    int file_size = 0;
    while (1) {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        file_size++;

        if (file_size == buffer_size - 1) {
            char* old_buffer = buffer;
            buffer = (char*)malloc(buffer_size * 2 * sizeof(char));

            for (int i = 0; i < file_size - 1; i++) {
                buffer[i] = old_buffer[i];
            }
            buffer[file_size - 1] = c;

            buffer_size *= 2;
            free(old_buffer);
        } else {
            buffer[file_size - 1] = c;
        }
    }
    buffer[file_size] = '\0';
    return buffer;
}

char* create_response(char* path) {
    char* file_string = read_file(path);

    char* MIME_type = "text/plain";
    if (strstr(path, ".css")) {
        MIME_type = "text/css";
    } else if (strstr(path, ".html")) {
        MIME_type = "text/html";
    } else if (strstr(path, ".json")) {
        MIME_type = "text/json";
    } else if (strstr(path, ".js")) {
        MIME_type = "text/javascript";
    }

    int content_length = strlen(file_string);

    char headers[150];

    sprintf(headers,
            "HTTP/1.1 %d OK\nServer: Anirvin's Server\nContent-Type: "
            "%s\nContent-Length: %d\nConnection: close\n\n",
            response_status, MIME_type, content_length);

    int len = strlen(headers) + strlen(file_string) + 1;
    char* response = (char*)malloc(strlen(headers) + strlen(file_string) + 1);
    int i;
    for (i = 0; i < strlen(headers); i++) {
        response[i] = headers[i];
    }

    for (int file_ptr = 0; file_ptr < strlen(file_string); file_ptr++) {
        response[i++] = file_string[file_ptr];
    }

    response[len - 1] = '\0';

    free(file_string);
    return response;
}

int open_webserver() {
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    struct addrinfo *results, *rp;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int sfd, s;
    s = getaddrinfo("localhost", "3000", &hints, &results);

    if (s != 0) {
        printf("failed %d\n", s);
        return 1;
    }

    for (rp = results; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd < 0) {
            continue;
        }
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){1},
                       sizeof(int)) < 0)
            return -1;
        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }
        close(sfd);
    }
    freeaddrinfo(results);

    if (rp == NULL) {
        fprintf(stderr, "Could not bind\n");
        return 1;
    }

    if (listen(sfd, 10) != 0) {
        return 1;
    }

    server_file_descriptor = sfd;

    struct sockaddr addr;
    socklen_t addrlen;
    int cfd;
    while (!sig_recieved && (cfd = accept(sfd, &addr, &addrlen)) != -1) {
        if (cfd == -1) {
            return 1;
        }

        char* request = (char*)malloc(MAX_REQUEST_SIZE + 1);
        read(cfd, request, MAX_REQUEST_SIZE);
        request[MAX_REQUEST_SIZE] = '\0';
        printf("REQUEST:\n%s\n", request);

        char* path = get_requested_resource_path(request);

        printf("PATH:\n%s\n", path);

        char* response = create_response(path);
        send(cfd, response, sizeof(char) * strlen(response), 0);

        free(request);
        free(response);
        // free(path);
    }
    close(sfd);
}

void sig_handler(int signum) {
    printf("\nCAUGHT\n");
    sig_recieved = true;
    if (server_file_descriptor != -1) {
        printf("Closing socket\n");
        close(server_file_descriptor);
        _exit(0);
    }
    _exit(1);
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);

    printf("Starting server...\n");

    open_webserver();
    return 0;
}
