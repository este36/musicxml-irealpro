#ifndef __DA_APPEND__
#define __DA_APPEND__

#define da_append(da, e) \
    do { \
        if (da.count >= da.capacity) { \
            if (da.capacity == 0) da.capacity = 256; \
            else da.capacity *= 2; \
            da.items = realloc(da.items, da.capacity*sizeof(*da.items)); \
        }\
        da.items[da.count++] = e;\
    } while (0) \

#endif
