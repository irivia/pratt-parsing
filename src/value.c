#include "value.h"
#include "arena.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

String string_create(const char *text, size_t len)
{
    String str;
    str.len = len;
    str.data = malloc(sizeof(char) * (str.len + 1));
    assert(text && len && "Text doesn't exist");
    memcpy(str.data, text, sizeof(char) * len);
    str.data[str.len] = '\0';

    return str;
}

void string_destroy(String *string)
{
    free(string->data);
    string->data = NULL;
    string->len = 0;
}

String string_create_arena(Arena *arena, const char *text, size_t len)
{
    String str;
    str.len = len;
    str.data = arena_alloc(arena, sizeof(char) * (str.len + 1));
    memcpy(str.data, text, sizeof(char) * len);
    str.data[str.len] = '\0';

    return str;
}

char* num_to_str(Value value)
{
    char *buffer = NULL;

    asprintf(&buffer, "%0.15Lf", AS_NUM(value));
    for (int i = strlen(buffer) - 1; i >= 0; i--) {
        if (buffer[i] != '0') break;
        buffer[i] = '\0';
    }
    int len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '.') {
        buffer[len - 1] = '\0';
    }

    return buffer;
}

void value_to_str(char **buffer, Value *value)
{
    *buffer = NULL;
    switch (value->type) {
        case VALUE_NUM:
            *buffer = num_to_str(*value);
            break;
        case VALUE_STR:
            asprintf(buffer, "%.*s", (int)AS_STR(*value).len, AS_STR(*value).data);
            break;
        case VALUE_BOOL:
            asprintf(buffer, "%s", AS_BOOL(*value) ? "true" : "false");
            break;
        case VALUE_NULL:
            asprintf(buffer, "null");
            break;
    }
}

char* value_type_to_str(ValueType type)
{
    switch (type) {
        case VALUE_NUM: return "Number";
        case VALUE_STR: return "String";
        case VALUE_BOOL: return "Bool";
        case VALUE_NULL: return "Null";
    }

    return NULL;
}

String string_add(Arena *arena, String *one, String *two)
{
    String str;
    str.len = one->len + two->len;
    str.data = arena_alloc(arena, str.len * sizeof(char));

    memcpy(str.data, one->data, one->len * sizeof(char));
    memcpy(&str.data[one->len], two->data, two->len * sizeof(char));

    return str;
}

bool string_compare(String* one, String *two)
{
    if (one->len == two->len) {
        return memcmp(one->data, two->data, one->len * sizeof(char)) == 0;
    }

    return false;
}

Value cast_value_to_str(Value val)
{
    if (val.type == VALUE_STR) return val;

    Value new_val = {0};

    value_to_str(&AS_STR(new_val).data, &val);
    AS_STR(new_val).len = strlen(AS_STR(new_val).data);
    new_val.type = VALUE_STR;

    return new_val;
}
