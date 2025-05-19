// #include <stdio.h>

#include "mongoose.h"
#include "mxl2irp.h"

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;

        if (mg_match(hm->uri, mg_str("/api"), NULL)) {
            mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m: %m}", MG_ESC("result"), MG_ESC("success"));
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "./public/"};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

int main() {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://0.0.0.0:5173", fn, NULL);
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    return 0;
}
