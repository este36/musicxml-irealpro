#include "da_append.h"
#include "mongoose.h"
#include "mxl2irp.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void parse_mg_str(const struct mg_str* ref, da_str* dest) {
    dest->buf = ref->buf;
    dest->cap = dest->len = ref->len;
}

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

static void terminate(struct mg_connection *c)
{
    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m: %m}", MG_ESC("result"), MG_ESC("server error"));
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        // MG_INFO(("EWWW"));

        if (mg_match(hm->uri, mg_str("/api/convert/"), NULL)) {

            mxl2irp_convert_params convert_params = {0};
            convert_params.include_only = DA_STR("");
            da_str res = {0};

            char buf[4];
            if (!mg_http_get_var(&hm->body, "part-number", buf, 4)) { terminate(c); return; }
            convert_params.part_number = atoi(buf);
            if (!mg_http_get_var(&hm->body, "include-only", convert_params.include_only.buf, DA_MIN_CAPACITY)) { terminate(c); return; } 
            if (!mg_http_get_var(&hm->body, "filename", convert_params.filename.buf, DA_MIN_CAPACITY)) { terminate(c); return; } 

            mxl2irp_get_url(&convert_params, &res);

            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m: %m}", MG_ESC("result"), MG_ESC(res.buf));

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
