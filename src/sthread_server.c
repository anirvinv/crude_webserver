#include "common.h"

int open_webserver();

void* handle_connection(void* arg);

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);

    printf("Starting server on port 3000\n");

    open_webserver();
    return 0;
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
        struct linger linger_opt;
        linger_opt.l_onoff = 1;
        linger_opt.l_linger = 0;  // immediate close
        if (setsockopt(sfd, SOL_SOCKET, SO_LINGER, &linger_opt,
                       sizeof(linger_opt)) < 0)
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

    while (!sig_recieved) {
        int cfd = accept(sfd, &addr, &addrlen);
        if (cfd == -1) {
            exit(EXIT_FAILURE);
        }
        printf("Connected to %d\n", cfd);

        int* p_cfd = (int*)malloc(sizeof(int));
        *p_cfd = cfd;

        handle_connection(p_cfd);
    }
    close(sfd);
}

void* handle_connection(void* arg) {
    int response_code = 200;

    int client_file_descriptor = *((int*)arg);

    free(arg);

    char* request = (char*)malloc(MAX_REQUEST_SIZE + 1);

    read(client_file_descriptor, request, MAX_REQUEST_SIZE);
    for (int i = 0; i < MAX_REQUEST_SIZE; i++) {
        if (request[i] < 0 || request[i] > 127) {
            request[i] = '\0';
            break;
        }
    }
    if (strstr(request, "GET") == NULL) {
        return NULL;
    }

    printf("REQUEST:\n%s\n\n", request);

    char* path = get_requested_resource_path(request);

    printf("RESOURCE PATH:\n%s\n\n", path);

    char* response = create_response(path, &response_code);

    send(client_file_descriptor, response, sizeof(char) * strlen(response), 0);
    free(request);
    free(response);
    free(path);
    close(client_file_descriptor);
    return NULL;
}