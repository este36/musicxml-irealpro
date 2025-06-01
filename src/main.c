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
        "<rest \"measure=\"yes\"/>\r\n"
        "<duration>4</duration>\r\n" 
        "<voice>1</voice>\r\n" 
        "</note>\r\n" 
        "</measure>\r\n";
    printf("%s", xml);
    return 0;
}
