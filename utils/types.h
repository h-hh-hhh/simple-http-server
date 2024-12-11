#ifndef _UTILS_TYPES_H
#define _UTILS_TYPES_H

typedef enum {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNKNOWN
} method_t;

typedef struct {
    unsigned int key_length;
    char *key;
    unsigned int value_length;
    char *value;
} string_pair_t;

typedef struct {
    method_t method;
    unsigned int path_length;
    char *path;
    unsigned int num_headers;
    string_pair_t *headers;
    unsigned int body_length;
    char *body;
} request_t;

typedef struct {
    int status_code;
    unsigned int num_headers;
    string_pair_t *headers;
    unsigned int body_length;
    char *body;
} response_t;

#endif