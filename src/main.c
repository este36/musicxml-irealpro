#define MXL2IRP_DEBUG

#ifndef MXL2IRP_DEBUG
#define printf(...) ((void)0)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "lib/da.h"
#include "lib/sax.h"
// #include "mxl2irp.h"

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
        case XML_CHARACTERS:
           return PARSER_CONTINUE; 
        case XML_UNSET:
        default: return PARSER_CONTINUE;
    }
}


int main() {
    // mxl2irp_load_parser();
    const char* xml = "<measure number=\"2\">\r\n"
        "    <harmony print-frame=\"no\">\r\n"
        "<root>\r\n" 
        "<root-step>C</root-step>\r\n" 
        "</root>\r\n" 
        "<kind>major</kind>\r\n" 
        "<degree>\r\n" 
        "<degree-value>2</degree-value>\r\n" 
        "<degree-alter>0</degree-alter>\r\n" 
        "<degree-type>add</degree-type>\r\n" 
        "\t\t</degree>\r\n" 
        "    </harmony>\r\n" 
        "<note>\r\n" 
        "<rest measure=\"yes\"/>\r\n"
        "<duration>4</duration>\r\n" 
        "<voice>1</voice>\r\n" 
        "</note>\r\n" 
        "</measure>\r\n";
    size_t len = strlen(xml);
    printf("%s \n len: %zu\n", xml, len);
    sax_parse_xml(fn, NULL, xml, len);
    return 0;
}
