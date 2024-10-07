#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ensure.h"

typedef struct CurlResponse {
    char *str;
    int len;
} CurlResponse;

// Initialize the string structure
void initResponse(CurlResponse *s) {
    s->len = 0;
    s->str = malloc(s->len + 1);  // Allocate initial memory
    if (s->str) {
        s->str[0] = '\0';           // Null-terminate the string
    }
}

void freeResponse(CurlResponse *s) {
    free(s->str);
}


size_t write_callback(void *ptr, size_t size, size_t nmemb, CurlResponse *data) {
    size_t new_len = data->len + size * nmemb; // Calculate new length
    data->str = realloc(data->str, new_len + 1);  // Reallocate memory
    if (data->str == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0; // Return 0 to indicate an error
    }

    memcpy(data->str + data->len, ptr, size * nmemb); // Copy the data
    data->str[new_len] = '\0';  // Null-terminate the string
    data->len = new_len;        // Update the length

    return size * nmemb; // Return the number of bytes processed
}

char* find_csrf(char *html) {
    char *current = strstr(html, "name=\"_csrf\"");
    if (current == NULL) return NULL;

    // go back to the start of the tag
    while (*current != '<') current--;
    current = strstr(current, "value=");
    if (current == NULL) return NULL;
    current += 7; // go to start of the csrf


    int i = 0;
    int size = 1;
    char *csrf = calloc(size, sizeof(*csrf));
    while (*current != '"') {
        if (size <= i) {
            size *= 2;
            csrf = realloc(csrf, size*sizeof(*csrf));
            if (csrf == NULL) return NULL;
        }
        csrf[i++] = *current;
        current++;
    }
    csrf[i] = '\0';
    
    return csrf;
}


char *grabCsrfToken() {
    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Curl: Init failure\n");

    char *header = "{\"user-agent\": \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chromium/80.0.3987.160 Chrome/80.0.3987.163 Safari/537.36\"}";

    curl_easy_setopt(curl, CURLOPT_URL, "https://themis.housing.rug.nl/log/in");

    CurlResponse response;
    initResponse(&response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_HEADER, header);


    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode result = curl_easy_perform(curl);

    ensuref(result == CURLE_OK, "Curl: failed\n");

    char *csrf = find_csrf(response.str);
    freeResponse(&response);
    curl_easy_cleanup(curl);
    
    return csrf;
}


void login(char *user, char *password, char *csrfToken) {

    CURL *curl = curl_easy_init();
    ensuref(curl != NULL, "Curl: init failure\n");

    char *header = "{\"user-agent\": \"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chromium/80.0.3987.160 Chrome/80.0.3987.163 Safari/537.36\"}";

    int dataSize = strlen(csrfToken) + 2*strlen(user) + strlen(password) + 100;
    char data[dataSize];
    sprintf(data, "{\"user\": \"%s\", \"password\": \"%s\", \"_csrf\": \"%s\", \"sudo\":\"%s\"}", user, password, csrfToken, user);

    // curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts");
    curl_easy_setopt(curl, CURLOPT_URL, "https://themis.housing.rug.nl/log/in");

    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    CurlResponse response;
    initResponse(&response);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_HEADER, header);


    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode result = curl_easy_perform(curl);
    
    ensuref(result == CURLE_OK, "Curl: failed\n");

    printf("%s\n", response.str);
    freeResponse(&response);
    curl_easy_cleanup(curl);

}

int themis() {
    char *csrfToken = grabCsrfToken();
    ensuref(csrfToken != NULL, "Curl: failed to grab csrf token\n");

    char *user = "user";
    char *password = "password";
    login(user, password, csrfToken);

    free(csrfToken);

    return 0;
}