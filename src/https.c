#include "https.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ensure.h"
#include "json.h"

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


void addHeaders(CURL *curl, jsonElement headersJson) {
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

CurlResponse *getCURL(char *url, jsonElement headersJson, cookies cookie, char *cookiePath) {

    CurlResponse *response = newResponse();

    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Curl: Init failure\n");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // cookies
    if (cookie == load || cookie == load_save) {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiePath); // Enable cookie engine
    }
    if (cookie == save || cookie == load_save) {
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath); // Enable cookie engine
    }

    // build headers
    struct curl_slist *headers = NULL;

    for (int i = 0; i < headersJson->length; i++) {
        char whomp[10000];
        jsonElement el = headersJson->elements[i];
        sprintf(whomp, "%s: %s", el->attribute, el->value_string);
        headers = curl_slist_append(headers, whomp);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


    // save data to response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    // verify ssl certificate
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);


    CURLcode result = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    ensuref(result == CURLE_OK, "Curl: failed\n");

    return response;
}

CurlResponse *postCURL(char *url, jsonElement headersJson, char* data, cookies cookie, char *cookiePath) {
    CurlResponse *response = newResponse();

    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Curl: init failure\n");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


    // cookies
    if (cookie == load || cookie == load_save) {
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiePath); // Enable cookie engine
    }
    if (cookie == save || cookie == load_save) {
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiePath); // Enable cookie engine
    }


    // build headers
    struct curl_slist *headers = NULL;

    for (int i = 0; i < headersJson->length; i++) {
        char temp[10000];
        jsonElement el = headersJson->elements[i];
        sprintf(temp, "%s: %s", el->attribute, el->value_string);
        headers = curl_slist_append(headers, temp);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


    // data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    // save data to response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    // verify ssl certificate
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);


    CURLcode result = curl_easy_perform(curl);
    ensuref(result == CURLE_OK, "Curl: failed\n");

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return response;
}
