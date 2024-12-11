#ifndef _UTILS_UTILS_H
#define _UTILS_UTILS_H

#include "types.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

request_t *create_request(const char *request_body);

void free_request(request_t *request);

response_t *create_response(int status_code, const char *body);

int add_header(response_t *response, const char *key, const char *value);

void free_response(response_t *response);

#endif