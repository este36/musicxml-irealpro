#ifndef __DA_APPEND__
#define __DA_APPEND__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifndef DA_MIN_CAPACITY
#define DA_MIN_CAPACITY 128
#endif
#define DA_UNSET (size_t)-1

// struct example {
//     size_t capacity;
//     size_t count;
//     void* items;
// };

#define da_append(da, e) \
    do { \
        if (da.count >= da.capacity) { \
            size_t new_cap = da.capacity ? da.capacity * 2 : DA_MIN_CAPACITY; \
            if (new_cap <= da.capacity) break; \
            da.capacity = new_cap; \
            da.items = realloc(da.items, da.capacity*sizeof(*da.items)); \
        }\
        if (da.items) da.items[da.count++] = e;\
        else da.capacity = da.count = 0; \
    } while (0)

#define da_free(da) \
    do { \
        free((da)->items); \
        (da)->count = (da)->capacity = 0; \
    } while (0) 

typedef struct da_str {
    char *buf;
    size_t len;
    size_t cap;
} da_str;

static inline da_str da_str_c(const char* c, size_t l)
{
    if (!c) return (da_str){0};

    da_str s = {0};
    s.len = l != DA_UNSET ? l :strlen(c);
    s.cap = s.len + 1;
    if (s.cap < DA_MIN_CAPACITY) s.cap = DA_MIN_CAPACITY;
    s.buf = malloc(s.cap);
    if (s.buf) {
        memcpy(s.buf, c, s.len);
        s.buf[s.len] = '\0';
    } else {
        s.cap = s.len = 0;
    }
    return s;
}

#define DA_STR(s) da_str_c(s, DA_UNSET)
#define DA_STRCPY(s, l) da_str_c(s, l)

static inline int str_append(da_str *s, const char *e) {
    size_t e_len = strlen(e);
    size_t needed = s->len + e_len + 1;

    if (needed > s->cap) {
        size_t new_cap = s->cap ? s->cap : DA_MIN_CAPACITY;
        while (new_cap < needed) new_cap *= 2;
        s->buf = (char *)realloc(s->buf, new_cap);
        if (!s->buf) return -1;
        s->cap = new_cap;
    }

    memcpy(s->buf + s->len, e, e_len);
    s->len += e_len;
    s->buf[s->len] = '\0';
    return 0;
}

static inline void da_str_free(da_str *s) {
    free(s->buf);
    s->buf = NULL;
    s->len = s->cap = 0;
}

// used for sub string, not null terminated
typedef struct da_string_ref {
    const char* buf;
    size_t len;
} da_string_ref;

void str_ref_copy(da_string_ref* ref, char* dest, size_t length) {
    size_t cpy_len = ref->len < length - 1 ? ref->len : length - 1;
    for (size_t i = 0; i < cpy_len; ++i) {
        dest[i] = ref->buf[i];
    }
    dest[cpy_len] = '\0';
}

bool str_ref_cmp(da_string_ref* a, da_string_ref* b)
{
    return a->len == b->len && memcmp(a->buf, b->buf, a->len) == 0;
}

#endif
