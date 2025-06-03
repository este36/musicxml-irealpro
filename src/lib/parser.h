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
INLINE int get_song_metadata(void* user_data, sax_context* context)
{
    return 0;
}

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
INLINE int get_irealpro_song_body(void* user_data, sax_context* context)
{
    return 0;
}

INLINE int parse_musicxml(da_str* irealpro_song, const char* musicxml, const size_t musicxml_length)
{

    // sax_lexer lexer = sax_lexer_init(buf, size);
    // sax_context context = sax_context_init(&lexer);
    // sax_parse_xml(fn, NULL, &context);
    return 0;
}

#endif // __PARSER_H__
