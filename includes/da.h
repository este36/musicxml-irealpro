#ifndef __DA_APPEND__
#define __DA_APPEND__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#ifndef DA_MIN_CAPACITY
#define DA_MIN_CAPACITY 64
#endif
#define DA_UNSET (size_t)-1

// struct example {
//     size_t capacity;
//     size_t count;
//     void* items;
// };

// Inspired by tsoding, the youtuber / twitch streamer
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

typedef struct
{
    char	*buf;
    size_t	len;
    size_t	cap;
}	t_dstr;

typedef struct
{
    const char	*buf;
    size_t		len;
}	t_str_ref;

#define DA_STR(s) da_str_c(s, DA_UNSET)
#define DA_STRCPY(s, l) da_str_c(s, l)
// Use this for evaluating string and string length at compile time
#define STR_REF(cstr) (t_str_ref){.buf = cstr, .len = (sizeof(cstr) - 1)}

t_dstr	dstr_c(const char* c, size_t l);
int		dstr_init(t_dstr *dst, size_t capacity);
int		dstrcat(t_dstr *dst, const char *src);
void	dstr_free(t_dstr *s);
void	str_ref_copy(const t_str_ref* ref, char* dest, size_t length);
bool	str_ref_eq(const t_str_ref* a, const t_str_ref* b);

#endif
