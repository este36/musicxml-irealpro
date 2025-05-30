#define MXL2IRP_DEBUG

#ifndef MXL2IRP_DEBUG
#define printf(...) ((void)0)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "da_append.h"
#include "mongoose.h"
#include "mxl2irp.h"

void parse_mg_str(const struct mg_str* ref, da_str* dest) {
    dest->buf = ref->buf;
    dest->cap = dest->len = ref->len;
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

            xmlUserData ud = {0};

            ud.params.include_only = DA_STR("");
            ud.params.filename = DA_STR("");
            da_str res = DA_STR(""); 
            char buf[4];
            int pn = 0;

            struct mg_str _pn = mg_http_var(hm->query, mg_str("part-number"));
            snprintf(buf, sizeof(buf),"%.*s", (int)_pn.len, _pn.buf);
            if (!parse_strict_int(buf, &pn)) { MG_ERROR(("ERR")); terminate(c); return; }
            ud.params.part_number = pn;

            struct mg_str _io = mg_http_var(hm->query, mg_str("include-only"));
            snprintf(ud.params.include_only.buf, DA_MIN_CAPACITY, "%.*s", (int)_io.len, _io.buf);

            struct mg_str _f = mg_http_var(hm->query, mg_str("filename"));
            snprintf(ud.params.filename.buf, DA_MIN_CAPACITY, "%.*s", (int)_f.len, _f.buf);

            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m: %m}", MG_ESC("result"), MG_ESC("eww"));

            ud.params.file.buf = hm->body.buf;
            ud.params.file.len = hm->body.len;

            MG_INFO(("file size: %d", ud.params.file.len));
            MG_INFO(("url size: %d", mxl2irp_get_url(&ud, &res)));

            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m: %m}", MG_ESC("result"), MG_ESC(res.buf));

            mxl2irp_free_xmlUserData(&ud);
            da_str_free(&res);
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
