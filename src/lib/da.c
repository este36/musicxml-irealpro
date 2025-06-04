#include "da.h"

da_str da_str_c(const char* c, size_t l)
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

int str_append(da_str *s, const char *e)
{
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

void da_str_free(da_str *s) 
{
    free(s->buf);
    s->buf = NULL;
    s->len = s->cap = 0;
}

void str_ref_copy(const da_str_ref* ref, char* dest, size_t length) 
{
    size_t cpy_len = ref->len < length - 1 ? ref->len : length - 1;
    for (size_t i = 0; i < cpy_len; ++i) {
        dest[i] = ref->buf[i];
    }
    dest[cpy_len] = '\0';
}

bool str_ref_cmp(const da_str_ref* a, const da_str_ref* b)
{
    return a->len == b->len && memcmp(a->buf, b->buf, a->len) == 0;
}
