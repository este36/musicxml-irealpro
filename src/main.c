#include "mongoose.h"
#include "mxl2irp.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int parse_strict_int(const char *str, int *out) {
    if (str == NULL || *str == '\0') return 0;

    char *end;
    errno = 0;

    long val = strtol(str, &end, 10);

    if (errno != 0 || *end != '\0') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;

    *out = (int) val;
    return 1;
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_match(hm->uri, mg_str("/api/convert"), NULL)) {

            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{}");

            struct mg_http_part part;
            size_t pos = 0;
            
            while ((pos = mg_http_next_multipart(hm->body, pos, &part)) != 0) {
              MG_INFO(("Chunk name: [%.*s] filename: [%.*s] length: %lu bytes",
                       part.name.len, part.name.buf,
                       part.filename.len, part.filename.buf, part.body.len));

              if (mg_strcmp(part.name, mg_str("file"))) {

              } else if (mg_strcmp(part.name, mg_str("part-number"))) {

              } else if (mg_strcmp(part.name, mg_str("include-only"))) {

              }
            }
            // char urlBuffer[MXL2IRP_URL_BUFFER_LEN] = {0};
            // xmlDocPtr xml = xmlParseMemory(hm->body.buf, strlen(hm->body.buf));

            // printf("%s", urlBuffer);
            // mxl2irp_get_url(xml, urlBuffer, part_number, include_only_buf);

            // xmlFreeDoc(xml);
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "./public/"};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

int main() {
    mxl2irp_load_parser();

    mg_log_set(MG_LL_DEBUG);
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    mg_http_listen(&mgr, "http://0.0.0.0:5173", fn, NULL);

    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }

    return 0;
}
