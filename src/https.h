#ifndef HTTPS_H
#define HTTPS_H

#include "json.h"

typedef struct CurlResponse {
    char *str;
    int len;
} CurlResponse;

CurlResponse *getCURL(char *url, jsonElement headersJson);
CurlResponse *postCURL(char *url, jsonElement headersJson, jsonElement dataJson);
void freeResponse(CurlResponse *response);

#endif