#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ensure.h"
#include "https.h"
#include "json.h"

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

    jsonElement header = newElement(_parent, "main");
    jsonElement agent = fromString("user-agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chromium/80.0.3987.160 Chrome/80.0.3987.163 Safari/537.36");
    
    addElement(header, agent);

    CurlResponse *r = getCURL("https://themis.housing.rug.nl/log/in", header);

    char *_csrf = find_csrf(r->str);

    freeResponse(r);
    freeElement(header);

    return _csrf;
}


void login(char *user, char *password, char *csrfToken) {

    jsonElement header = newElement(_parent, "main");
    jsonElement agent = fromString("user-agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chromium/80.0.3987.160 Chrome/80.0.3987.163 Safari/537.36");
    
    addElement(header, agent);

    jsonElement data = newElement(_parent, "");

    addElements(data, 4, fromString("user", user), fromString("password", password), fromString("_csrf", csrfToken), fromString("sudo", user));

    CurlResponse *r = postCURL("https://themis.housing.rug.nl/log/in", header, data);
    // CurlResponse *r = postCURL("https://jsonplaceholder.typicode.com/posts/", header, data);

    printf("%s\n", r->str);
    freeResponse(r);
    freeElement(data);
    freeElement(header);

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