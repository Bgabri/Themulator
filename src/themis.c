#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

#include "ensure.h"
// typedef struct {
//     char *username;
//     char *password;
//     char *
// } User;

// typedef enum Request {
//     GET, POST, PUT, DELETE
// } request;

// typedef struct {
//     Request request;
//     char *data;
//     // char *
// } HTTP;

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t real_size = size * nmemb;
    fwrite(ptr, size, nmemb, (FILE *)userdata);
    return real_size;
}

int themis() {
    // login request
    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Failed to initialise curl\n");


    char *header = "{\"user-agent\": \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chromium/80.0.3987.160 Chrome/80.0.3987.163 Safari/537.36\"}";

    // curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");
    curl_easy_setopt(curl, CURLOPT_URL, "https://themis.housing.rug.nl/log/in");


    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADER, header);

    // curl_easy_setopt(curl, CURLOPT_daga )
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode result = curl_easy_perform(curl);

    ensuref(result == CURLE_OK, "failed to access\n");

    curl_easy_cleanup(curl);
    return 0;
}