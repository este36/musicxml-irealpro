#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#include "lib/da.h"
#include "lib/sax.h"
// #include "mxl2irp.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#endif


int fn(void *user_data, sax_context * context)
{
    const xml_node* n = &context->found;

    switch (context->found.type) {
        case XML_TAG_OPEN:
            printf("open tag. name: %.*s\n", (int)n->target.len, n->target.buf);
            return PARSER_CONTINUE | NODE_IS_PARENT;
        case XML_TAG_CLOSE:
            printf("close tag. name: %.*s\n", (int)n->target.len, n->target.buf);
           return PARSER_CONTINUE; 
        case XML_SELF_CLOSING:
            printf("self_closing tag. name: %.*s\n", (int)n->target.len, n->target.buf);
           return PARSER_CONTINUE; 
        case XML_UNSET:
        default: return PARSER_CONTINUE;
    }
}

FILE *fopenf(const char *mode, const char *fmt, ...) {
    char path[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(path, sizeof(path), fmt, args);
    va_end(args);
    return fopen(path, mode);
}


int main() {
    // chords.musicxml
    // complicité.musicxml
    // DaysOfWineRoses.musicxml
    // Grace_and_Mercy.musicxml
    // Misty.musicxml
    // Out_of_Nothing.musicxml
    // sauts.musicxml
    // timesignature.musicxml
    // Unrealised_Love.musicxml
     FILE *f = fopen("tests/musicxml/DaysOfWineRoses.musicxml", "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = malloc(size + 1); 
    if (!buf) {
        perror("malloc");
        fclose(f);
        return 1;
    }

    fread(buf, 1, size, f);
    buf[size] = '\0';

    sax_lexer lexer = sax_lexer_init(buf, size);
    sax_context context = sax_context_init(&lexer);
    sax_parse_xml(fn, NULL, &context);
    free(buf);
    fclose(f);
    return 0;
}
