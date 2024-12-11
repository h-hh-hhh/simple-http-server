#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

request_t *create_request(const char *request_body) {
    int offset = 0;

    request_t *request = malloc(sizeof(request_t));

    request->method = UNKNOWN;
    char *method = malloc((7 + 1) * sizeof(char)); // longest possible request method is 7 bytes long
    sscanf(request_body + offset, "%7s", method);
    if (!strcmp(method, "GET")) {
        request->method = GET;
    }
    else if (!strcmp(method, "HEAD")) {
        request->method = HEAD;
    }
    else if (!strcmp(method, "POST")) {
        request->method = POST;
    }
    else if (!strcmp(method, "PUT")) {
        request->method = PUT;
    }
    else if (!strcmp(method, "DELETE")) {
        request->method = DELETE;
    }
    else if (!strcmp(method, "CONNECT")) {
        request->method = CONNECT;
    }
    else if (!strcmp(method, "OPTIONS")) {
        request->method = OPTIONS;
    }
    else if (!strcmp(method, "TRACE")) {
        request->method = TRACE;
    }
    else if (!strcmp(method, "PATCH")) {
        request->method = PATCH;
    }
    offset += strlen(method) + 1;

    request->path = malloc((255 + 1) * sizeof(char)); // path limited to 255 chars in this case
    sscanf(request_body + offset, "%255s", request->path);
    request->path_length = strlen(request->path);
    offset += request->path_length + 11; // 11 is the length of " HTTP/x.x\r\n"

    // headers
    request->headers = NULL;
    request->num_headers = 0;
    while (strstr(request_body + offset, "\r\n\r\n") - request_body > offset) {
        if (request->headers) {
            request->headers = realloc(request->headers, (++(request->num_headers)) * sizeof(string_pair_t));
        }
        else { // happens right after creation
            request->headers = malloc(1 * sizeof(string_pair_t));
            request->num_headers = 1;
        }
        string_pair_t *header = request->headers + request->num_headers - 1;
        header->key = malloc(BUFSIZ * sizeof(char));
        header->value = malloc(BUFSIZ * sizeof(char));
        //sscanf(request_body + offset, "%8192s: %8192s\r\n", header->key, header->value);
        sscanf(request_body + offset, "%8191[^:]: %255[^\r\n]", header->key, header->value);
        header->key_length = strlen(header->key);
        header->value_length = strlen(header->value);
        offset += header->key_length + 2 + header->value_length + 2; // 2 is ": ", 1 is "\r\n"
    }

    offset += 2; // catch separating "\r\n"
    request->body_length = strlen(request_body + offset);
    request->body = malloc((request->body_length + 1) * sizeof(char));
    memcpy(request->body, request_body + offset, request->body_length + 1);
    
    free(method);
    return request;
}

void free_request(request_t *request) {
    if (!request) {
        return; // Nothing to free
    }

    // free headers contents
    if (request->headers) {
        for (int i = 0; i < request->num_headers; i++) {
            free(request->headers[i].key);
            free(request->headers[i].value);
        }
        free(request->headers);
    }
    if (request->path) {
        free(request->path);
    }
    if (request->body) {
        free(request->body);
    }
    free(request);
}

response_t *create_response(int status_code, const char *body) {
    response_t *response = malloc(sizeof(response_t));

    response->status_code = status_code;
    response->headers = NULL; // leave as null for now, will be populated later with add_header
    response->num_headers = 0;
    response->body_length = strlen(body);
    response->body = malloc((response->body_length + 1) * sizeof(char));
    memcpy(response->body, body, response->body_length + 1);

    return response;
}

int add_header(response_t *response, const char *key, const char *value) {
    if (response->headers) {
        response->headers = realloc(response->headers, (++(response->num_headers)) * sizeof(string_pair_t));
    }
    else { // happens right after creation
        response->headers = malloc(1 * sizeof(string_pair_t));
        response->num_headers = 1;
    }

    string_pair_t *header = response->headers + response->num_headers - 1;

    header->key_length = strlen(key);
    header->key = malloc((header->key_length + 1) * sizeof(char));
    memcpy(header->key, key, header->key_length + 1);

    header->value_length = strlen(value);
    header->value = malloc((header->value_length + 1) * sizeof(char));
    memcpy(header->value, value, header->value_length + 1);

    return 0; // success
}

void free_response(response_t *response) {
    if (!response) {
        return; // Nothing to free
    }

    // free headers contents
    if (response->headers) {
        for (int i = 0; i < response->num_headers; i++) {
            free(response->headers[i].key);
            free(response->headers[i].value);
        }
        free(response->headers);
    }
    if (response->body) {
        free(response->body);
    }
    free(response);
}