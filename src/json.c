#include "json.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jsonElement fromString(char *attribute, char *value) {
    jsonElement element = newElement(_string_prim, attribute);
    element->value_string = calloc(strlen(value) + 1, sizeof(char));
    strcpy(element->value_string, value);
    return element;
}

jsonElement fromInt(char *attribute, int value) {
    jsonElement element = newElement(_int_prim, attribute);
    element->value_int = value;
    return element;
}

jsonElement fromBool(char *attribute, int value) {
    jsonElement element = newElement(_bool_prim, attribute);
    element->value_int = value;
    return element;
}

jsonElement newElement(jsonType type, char *attribute) {
    jsonElement element = calloc(1, sizeof(*element));
    element->type = type;

    element->attribute = calloc(strlen(attribute) + 1, sizeof(char));
    strcpy(element->attribute, attribute);

    element->allocSize = 0;
    element->length = 0;
    return element;
}

void addElement(jsonElement parent, jsonElement child) {
    if (parent->allocSize <= 0) {
        parent->allocSize = 1;
        parent->length = 0;
        parent->elements = calloc(parent->allocSize, sizeof(jsonElement));
    }
    if (parent->length + 1 >= parent->allocSize) {
        parent->allocSize *= 2;
        parent->elements = realloc(parent->elements, parent->allocSize * sizeof(jsonElement));
    }

    parent->elements[parent->length++] = child;
}

void addElements(jsonElement parent, int count, ...) {
    if (parent->allocSize <= 0) {
        parent->allocSize = 1;
        parent->length = 0;
        parent->elements = calloc(parent->allocSize, sizeof(jsonElement));
    }
    if (parent->length + count >= parent->allocSize) {
        parent->allocSize += count;
        parent->elements = realloc(parent->elements, parent->allocSize * sizeof(jsonElement));
    }

    va_list childes;
    va_start(childes, count);

    for (; count > 0; count--) {
        parent->elements[parent->length++] = va_arg(childes, jsonElement);
    }

    va_end(childes);
}

char *toString(jsonElement element) {
    char *val = calloc(10000, sizeof(char *));

    char *delim_open = "{";
    char *delim_close = "}";

    switch (element->type) {
        case _null:
            sprintf(val, "null");
            break;
        case _int_prim:
            sprintf(val, "%d", element->value_int);
            break;
        case _bool_prim:
            if (element->value_int) {
                sprintf(val, "true");
            } else {
                sprintf(val, "false");
            }
            break;
        case _string_prim:
            sprintf(val, "\"%s\"", element->value_string);
            break;
        case _element_array:
            delim_open = "[";
            delim_close = "]";
        case _object:
        case _parent:
            sprintf(val, "%s", delim_open);
            for (int i = 0; i < element->length; i++) {
                char *s = toString(element->elements[i]);
                if (i) strcat(val, ", ");
                strcat(val, s);
                free(s);
            }
            strcat(val, delim_close);
            break;
        case _prim_array:
            break;
    }

    char *buf = calloc(1000, sizeof(char *));
    if (element->type == _parent) {
        sprintf(buf, "%s", val);
    } else {
        sprintf(buf, "\"%s\": %s", element->attribute, val);
    }
    free(val);

    return buf;
}

void freeElement(jsonElement element) {
    free(element->attribute);

    switch (element->type) {
        case _string_prim:
            free(element->value_string);
            break;
        case _element_array:
        case _object:
        case _parent:
        case _prim_array:
            for (int i = 0; i < element->length; i++) {
                freeElement(element->elements[i]);
            }
            free(element->elements);
            break;
        case _int_prim:
        case _bool_prim:
        case _null:
            break;
    }

    free(element);
}
