#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// #include "lib/da.h"
// #include "lib/sax.h"
// #include "mxl2irp.h"
#include "lib/parser.h"

#ifndef DEBUG
#define printf(...) ((void)0)
#endif

char* open_file(long* length, const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    *length = size;
    rewind(f);

    char *buf = malloc(size + 1); 
    if (!buf) {
        perror("malloc");
        fclose(f);
        return NULL;
    }

    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return buf;
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

    long file_len = 0;
    // char* file = open_file(&file_len, "tests/musicxml/DaysOfWineRoses.musicxml");
    char* file = open_file(&file_len, "tests/musicxml/Out_of_Nothing.musicxml");

    if (!file) return 1;

    IrpSong irpSong = {0};
    
    if (parse_musicxml_song(&irpSong, NULL, file, file_len)) {
        free(file);
        return 1;
    }

    printf("Song title: %s, composer: %s\n", irpSong.title, irpSong.composer);

    free(file);
    return 0;
}
