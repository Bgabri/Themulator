#ifndef HTTPS_H
#define HTTPS_H

#include "json.h"

typedef enum cookies {
    load, save, load_save
} cookies;

typedef struct CurlResponse {
    char *str;
    int len;
} CurlResponse;

CurlResponse *getCURL(char *url, jsonElement headersJson, cookies cookie, char *cookiePath);
CurlResponse *postCURL(char *url, jsonElement headersJson, char* data, cookies cookie, char *cookiePath);
void freeResponse(CurlResponse *response);

#endif
