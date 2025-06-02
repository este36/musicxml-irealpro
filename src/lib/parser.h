#ifndef __MXL_PARSER_H__
#define __MXL_PARSER_H__

#include "sax.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#define INLINE static inline
#else
#define INLINE static
#endif

// TODO
INLINE int get_parts_count(void* user_data, sax_context* context)
{
    return 0;
}

// TODO
INLINE int find_part(void* user_data, sax_context* context)
{
    return 0;
}

// TODO
INLINE int build_irealpro_song(void* user_data, sax_context* context)
{
    return 0;
}

#endif // __PARSER_H__
