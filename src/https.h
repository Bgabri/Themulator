#ifndef HTTPS_H
#define HTTPS_H

#include "json.h"

#define COOKIE_FILE "cookies.txt"

typedef enum cookies {
    load, save, load_save
} cookies;

typedef struct CurlResponse {
    char *str;
    int len;
} CurlResponse;


CurlResponse *getCURL(char *url, jsonElement headersJson, cookies cookie);
CurlResponse *postCURL(char *url, jsonElement headersJson, char* data, cookies cookie);
void freeResponse(CurlResponse *response);

#endif