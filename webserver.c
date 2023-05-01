#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

char* read_file(char* file_name) {
    FILE* fp = fopen(file_name, "r");

    if (fp == NULL) {
        char* buffer = (char*)malloc(20 * sizeof(char));
        char* not_found_msg = "Page not found";
        int i = 0;
        for (; i < strlen(not_found_msg); i++) {
            buffer[i] = not_found_msg[i];
        }
        buffer[i] = '\0';
        return buffer;
    }
    int buffer_size = 2;
    char* buffer = (char*)malloc(2 * sizeof(char));
    char c;
    int file_size = 0;
    while (1) {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
        if (c == '\n') continue;
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

char* create_response(char* file_name) {
    char* file_string = read_file(file_name);

    int content_length = strlen(file_string);

    char headers[150];

    sprintf(headers,
            "HTTP/1.1 200 OK\nServer: Anirvin's Server\nContent-Type: "
            "text/html\nContent-Length: %d\nConnection: close\n\n",
            content_length);

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

    struct sockaddr addr;
    socklen_t addrlen;

    int cfd = accept(sfd, &addr, &addrlen);

    if (cfd == -1) {
        return 1;
    }

    char* request = (char*)malloc(1501);
    read(cfd, request, 1500);
    request[1500] = '\0';
    printf("%s", request);

    char* response = create_response("./html/something.html");
    send(cfd, response, sizeof(char) * strlen(response), 0);

    free(request);
    free(response);
    close(sfd);
}

int main() { open_webserver(); }
