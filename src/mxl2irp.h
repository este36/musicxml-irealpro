#ifndef __MXL2IRP_H__
#define __MXL2IRP_H__
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "uthash.h"

typedef struct {
    int* items;
    size_t count;
    size_t capacity;
} Numbers;

#define da_append(da, e) \
    do { \
        if (da.count >= da.capacity) { \
            if (da.capacity == 0) da.capacity = 256; \
            else da.capacity *= 2; \
            da.items = realloc(da.items, da.capacity*sizeof(*da.items)); \
        }\
        da.items[da.count++] = e;\
    } while (0) \

int mxl2irp_load_parser()
{
    return 0;
}

#endif // __MXL2IRP_H__
