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
    char* file = open_file(&file_len, "musicxml/DaysOfWineRoses.musicxml");
    // char* file = open_file(&file_len, "musicxml/Out_of_Nothing.musicxml");

    if (!file) return 1;

    IrpSong irpSong = {0};
    
    if (parse_musicxml_song(&irpSong, STR_REF("P1"), file, file_len)) {
        free(file);
        return 1;
    }

    printf("Song title: %s, composer: %s\n", irpSong.title, irpSong.composer);
    printf("Number of bars: %zu\n", irpSong.measures.count);

    for (size_t i = 0; i < irpSong.measures.count; i++) {
        printf("measure %zu\n", i);
        for (size_t j = 0; j < irpSong.measures.items[i].chords.count; j++) {
            printf("    chord %zu\n", j);
            printf("        duration:\t%f\n", irpSong.measures.items[i].chords.items[j].duration);
            printf("        root:\t\t%s\n", get_note_str(irpSong.measures.items[i].chords.items[j].root));
        }
    }

    free(file);
    return 0;
}
