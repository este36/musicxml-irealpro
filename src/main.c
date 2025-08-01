#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "parser.h"

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
    long file_len = 0;
    // char* file = open_file(&file_len, "musicxml/chords.musicxml");
    // char* file = open_file(&file_len, "musicxml/complicité.musicxml");
    char* file = open_file(&file_len, "musicxml/DaysOfWineRoses.musicxml");
    // char* file = open_file(&file_len, "musicxml/Grace_and_Mercy.musicxml");
    // char* file = open_file(&file_len, "musicxml/Misty.musicxml");
    // char* file = open_file(&file_len, "musicxml/test.musicxml");
    // char* file = open_file(&file_len, "musicxml/Out_of_Nothing.musicxml");
    // char* file = open_file(&file_len, "musicxml/sauts.musicxml");
    // char* file = open_file(&file_len, "musicxml/timesignature.musicxml");
    // char* file = open_file(&file_len, "musicxml/Unrealised_Love.musicxml");

    if (!file) return 1;
    t_irealpro_song irpSong = {0};
    if (parse_musicxml_song(&irpSong, STR_REF("P1"), file, file_len) != 0) {
		printf("PARSER_STOP_ERROR\n");
        free(file);
        return 1;
    }
	// printf("ZOOM: %d\n", irpSong.zoom);
	char *url = irp_get_song_html(&irpSong);
	printf("%s", "<h1>\n\t");
	printf("Empty bars: %d\n", irpSong.first_empty_bars);
	printf("%s", "</h1>\n");
	printf("%s", "<h1>\n\t");
	printf("%s\n", url);
	printf("%s", "</h1>");
	free(url);
    // printf("Song title: %s, composer: %s\n", irpSong.title, irpSong.composer);
    // printf("Number of bars: %zu\n", irpSong.measures.count);
    // printf("Key: %s\n", get_note_str(irpSong.key));

    // for (size_t i = 0; i < irpSong.measures.count; i++) {
    //     printf("measure no %zu\n", i);
    //     printf("barlines: \"%c", irpSong.measures.items[i].barlines[0]);
    //     printf("%c\"\n", irpSong.measures.items[i].barlines[1]);
	// 	printf("rehearsal: \"%s\", ", irpSong.measures.items[i].rehearsal);
	// 	printf("playback: \"%s\"\n", irpSong.measures.items[i].playback);
	// 	printf("ending: %d, ", irpSong.measures.items[i].ending);
	// 	printf("segno: %d, ", irpSong.measures.items[i].is_segno);
	// 	printf("coda: %d, ", irpSong.measures.items[i].is_coda);
    //     printf("divisions: %d\n", irpSong.measures.items[i].divisions);
    //     printf("time_signate.beats: %d, ", irpSong.measures.items[i].time_signature.beats);
    //     printf("time_signate.beat_type: %d\n", irpSong.measures.items[i].time_signature.beat_type);
    //     for (size_t j = 0; j < irpSong.measures.items[i].chords.count; j++) {
    //         printf("    chord %zu\n", j);
    //         printf("        duration:\t%f\n", irpSong.measures.items[i].chords.items[j].duration);
    //         printf("        root:\t\t\"%s\"\n", get_note_str(irpSong.measures.items[i].chords.items[j].root));
    //         printf("        bass:\t\t\"%s\"\n", get_note_str(irpSong.measures.items[i].chords.items[j].bass));
    //         printf("        qual:\t\t\"%s\"\n", irpSong.measures.items[i].chords.items[j].quality);
    //     }
    // }

	irp_song_free(&irpSong);
    free(file);
    return 0;
}
