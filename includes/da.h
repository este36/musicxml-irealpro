#ifndef __DA_APPEND__
#define __DA_APPEND__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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

#ifndef DSTR_MIN_CAPACITY
#define DSTR_MIN_CAPACITY 256
#endif

t_dstr	dstr_from_cstr(const char *str);
t_dstr	dstr_from_str_ref(const char *str, size_t len);
int		dstr_init(t_dstr *dst, size_t capacity);
int		dstrcat(t_dstr *dst, const char *src);
int		dstrncat(t_dstr *dst, const char *src, size_t len);
int		dstr_resize(t_dstr *s, size_t new_len);
void	dstr_free(t_dstr *s);

typedef struct
{
    const char	*buf;
    size_t		len;
}	t_str_ref;

// Use this for evaluating string and string length at compile time
#define STR_REF(cstr) (t_str_ref){.buf = cstr, .len = (sizeof(cstr) - 1)}

t_str_ref	str_ref_from_cstr(const char *s);
void		str_ref_copy(char *dest, size_t dest_cap, const t_str_ref *src);
bool		str_ref_eq(const t_str_ref *a, const t_str_ref *b);

typedef struct
{
	void	*data;
	size_t	len;
	size_t	cap;
	size_t	_el_size;
}	t_darr;

#ifndef DARR_MIN_CAPACITY
#define DARR_MIN_CAPACITY 64
#endif

#define	darr_get(a, index) ((index) < (a->len) ? (a)->data + (index) * (a)->_el_size : (void *)0)

int			darr_init(t_darr *a, size_t cap, size_t el_size);
int			darr_push(t_darr *a, void *el);

#endif
