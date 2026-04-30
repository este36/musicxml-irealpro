
#include "da.h"
#include <string.h>
#include <stdlib.h>

t_str_ref str_ref_from_cstr(const char *s)
{
	return (t_str_ref){.buf = s, .len = (s != NULL ? strlen(s) : 0)};
}

void str_ref_copy(char *dest, size_t dest_cap, const t_str_ref *src) 
{
    size_t cpy_len = src->len < dest_cap - 1 ? src->len : dest_cap - 1;
	memcpy(dest, src->buf, cpy_len);
    dest[cpy_len] = '\0';
}

bool str_ref_eq(const t_str_ref* a, const t_str_ref* b)
{
    return a->len == b->len && memcmp(a->buf, b->buf, a->len) == 0;
}
