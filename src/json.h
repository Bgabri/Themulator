
typedef enum jsonType {
    _int_prim, _string_prim, _bool_prim, _element_array, _prim_array, _object, _null, _parent
} jsonType;

// typedef struct jsonData *json;
typedef struct jsonData *jsonElement;

typedef struct jsonData {
    jsonType type;
    char* attribute;
    int value_int;
    char* value_string;

    int allocSize;
    int length;
    jsonElement *elements;

} jsonData;

jsonElement fromString(char *attribute, char *value);
jsonElement fromInt(char *attribute, int value);
jsonElement fromBool(char *attribute, int value);
jsonElement newElement(jsonType type, char *attribute);
void addElement(jsonElement parent, jsonElement child);
void addElements(jsonElement parent, int count, ...);
char *toString(jsonElement element);
void freeElement(jsonElement element);