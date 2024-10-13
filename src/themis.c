#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ensure.h"
#include "https.h"
#include "io.h"
#include "json.h"
#include "parseArgs.h"

extern Options options;

char *find_csrf(char *html) {
    char *current = strstr(html, "name=\"_csrf\"");
    if (current == NULL) return NULL;

    // go back to the start of the tag
    while (*current != '<') current--;
    current = strstr(current, "value=");
    if (current == NULL) return NULL;
    current += 7;  // go to start of the csrf
    int i = 0;
    int size = 1;
    char *csrf = calloc(size, sizeof(*csrf));
    while (*current != '"') {
        if (size <= i) {
            size *= 2;
            csrf = realloc(csrf, (size + 1) * sizeof(*csrf));
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
    jsonElement agent = fromString("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0");

    addElement(header, agent);

    CurlResponse *r = getCURL("https://themis.housing.rug.nl/log/in", header, save);

    char *_csrf = find_csrf(r->str);

    freeResponse(r);
    freeElement(header);

    return _csrf;
}

int login(char *user, char *password, char *csrfToken) {
    jsonElement header = newElement(_parent, "main");
    jsonElement agent = fromString("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0");
    jsonElement referrer = fromString("Referer", "https://themis.housing.rug.nl/log/in");

    addElement(header, agent);
    addElement(header, referrer);

    char *data = calloc(strlen(user) + strlen(password) + strlen(csrfToken) + 100, sizeof(char));

    sprintf(data, "user=%s&password=%s&_csrf=%s", user, password, csrfToken);

    CurlResponse *r = postCURL("https://themis.housing.rug.nl/log/in", header, data, load_save);

    char *v = strstr(r->str, "Welcome, logged in as");
    
    int result = v != NULL;
    free(data);
    freeResponse(r);
    freeElement(header);
    return result;
}

CurlResponse *browse(char *url) {
    jsonElement header = newElement(_parent, "main");
    jsonElement agent = fromString("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0");

    addElement(header, agent);

    CurlResponse *r = getCURL(url, header, load);
    freeElement(header);
    return r;
}

char **getTestCases(char *assignment, int *tcsSize) {
    char *url = calloc(strlen(assignment) + 100, sizeof(char));
    sprintf(url, "https://themis.housing.rug.nl/%s", assignment);

    CurlResponse *r = browse(url);
    char *current = r->str;

    int tcsPos = 0;
    int tcsAllocSize = 1;
    char **tcs = calloc(tcsAllocSize, sizeof(char *));

    while ((current = strstr(current, "/%40tests/"))) {
        current += sizeof("/%40tests/") - 1;
        int i = 0;
        int size = 1;
        char *tc = calloc(size, sizeof(*tc));
        while (*current != '"') {
            if (size <= i) {
                size *= 2;
                tc = realloc(tc, (size + 1) * sizeof(*tc));
                if (tc == NULL) return NULL;
            }
            tc[i++] = *current;
            current++;
        }
        tc[i] = '\0';

        // printf("%s\n", tc);

        if (tcsAllocSize <= tcsPos) {
            tcsAllocSize *= 2;
            tcs = realloc(tcs, tcsAllocSize * sizeof(char *));
            if (tcs == NULL) return NULL;
        }

        tcs[tcsPos++] = tc;
    }

    free(url);
    freeResponse(r);
    *tcsSize = tcsPos;
    return tcs;
}

void downloadTestCases(char *assignment) {
    if (startsWith(assignment, "https://themis.housing.rug.nl")) assignment += strlen("https://themis.housing.rug.nl");
    if (startsWith(assignment, "themis.housing.rug.nl")) assignment += strlen("themis.housing.rug.nl");
    if (*assignment == '/') assignment++;

    

    int size = 0;
    char **tcs = getTestCases(assignment, &size);

    if (size == 0) return;
    char *url = calloc(sizeof(tcs[0]) + sizeof(assignment) + 100, sizeof(char));
    char *filePath = calloc(sizeof(tcs[0]) + sizeof(options.inDir) + sizeof(options.dir) + 100, sizeof(char));

    sprintf(filePath, "%s/%s", options.dir, options.refDir);
    createDir(filePath);
    sprintf(filePath, "%s/%s", options.dir, options.inDir);
    createDir(filePath);
    assignment += sizeof("course");
    for (int i = 0; i < size; i++) {
        if (endsWith(tcs[i], ".out")) {
            sprintf(filePath, "%s/%s/%s", options.dir, options.refDir, tcs[i]);
        } else {
            sprintf(filePath, "%s/%s/%s", options.dir, options.inDir, tcs[i]);
        }

        printf("\x1b[1m[ ]\x1b[0m Downloading:\x1b[2m %s\n\x1b[0m", filePath);
        fflush(stdout);

        sprintf(url, "https://themis.housing.rug.nl/file/%s/%%40tests/%s", assignment, tcs[i]);
        CurlResponse *r = browse(url);

        FILE *file = fopen(filePath, "w");

        printf("\x1b[1F\x1b[2K");
        if (file == NULL) {
            printf("\x1b[35m\x1b[1m[e]\x1b[0m Downloading:\x1b[2m %s: failed\n\x1b[0m", filePath);
            continue;
        }
        fprintf(file, "%s", r->str);
        fclose(file);

        printf("\x1b[32m\x1b[1m[o]\x1b[0m Downloaded:\x1b[2m %s\n\x1b[0m", filePath);

        freeResponse(r);
        free(tcs[i]);
    }
    free(url);
    free(filePath);
    free(tcs);
}

int themis() {
    int size = 0;
    printf("user name:\n");
    char *user = scanString(&size, '\n', 0);
    // char *user = "s4878728";

    printf("password:\n");
    char *pass = scanString(&size, '\n', '*');
    // char *pass = "Dumpster-Related-Pasta-Attempt-42";

    curl_global_init(CURL_GLOBAL_DEFAULT);

    char *csrfToken = grabCsrfToken();
    ensuref(csrfToken != NULL, "Curl: failed to grab csrf token\n");
    if (login(user, pass, csrfToken)) {
        fprintf(stderr, "Login: success\n");

        printf("Enter course link or path:\n");
        char *path = scanString(&size, '\n', 0);
        downloadTestCases(path);
        free(path);
    } else {
        fprintf(stderr, "Login: failed\n");
    }

    free(csrfToken);
    curl_global_cleanup();

    free(user);
    free(pass);

    // https://themis.housing.rug.nl/course/2024-2025/advalgo/labs-wk6/familytree

    return 0;
}