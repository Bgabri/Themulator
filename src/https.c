#include "https.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "ensure.h"

CurlResponse *newResponse() {
    CurlResponse *s = calloc(1, sizeof(*s));
    s->len = 0;
    s->str = malloc(s->len + 1);
    if (s->str) {
        s->str[0] = '\0';
    }
    return s;
}

void freeResponse(CurlResponse *response) {
    free(response->str);
    free(response);
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, CurlResponse *data) {
    size_t new_len = data->len + size * nmemb;
    data->str = realloc(data->str, new_len + 1);
    if (data->str == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy(data->str + data->len, ptr, size * nmemb);
    data->str[new_len] = '\0';
    data->len = new_len;

    return size * nmemb;
}

CurlResponse *getCURL(char *url, jsonElement headersJson) {
    char *header = toString(headersJson);
    CurlResponse *response = newResponse();

    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Curl: Init failure\n");

    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    curl_easy_setopt(curl, CURLOPT_HEADER, header);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode result = curl_easy_perform(curl);

    ensuref(result == CURLE_OK, "Curl: failed\n");

    curl_easy_cleanup(curl);

    free(header);
    return response;
}

CurlResponse *postCURL(char *url, jsonElement headersJson, jsonElement dataJson) {

    char *header = toString(headersJson);
    char *data = toString(dataJson);
    // printf("%s\n", header);
    // printf("%s\n", data);
    CurlResponse *response = newResponse();

    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Curl: init failure\n");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    // curl_easy_setopt(curl, CURLOPT_HEADER, header);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode result = curl_easy_perform(curl);
    ensuref(result == CURLE_OK, "Curl: failed\n");

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    free(header);
    free(data);
    return response;
}
