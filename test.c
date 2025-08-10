#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

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
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
    long file_len = 0;
    // char* file = open_file(&file_len, "musicxml/chords.musicxml");
    // char* file = open_file(&file_len, "musicxml/complicité.musicxml");
    // char* file = open_file(&file_len, "musicxml/DaysOfWineRoses.musicxml");
    // char* file = open_file(&file_len, "musicxml/Grace_and_Mercy.musicxml");
    // char* file = open_file(&file_len, "musicxml/Misty.musicxml");
    // char* file = open_file(&file_len, "musicxml/test.musicxml");
    // char* file = open_file(&file_len, "musicxml/Out_of_Nothing.musicxml");
    // char* file = open_file(&file_len, "musicxml/sauts.musicxml");
    // char* file = open_file(&file_len, "musicxml/timesignature.musicxml");
    // char* file = open_file(&file_len, "musicxml/Unrealised_Love.musicxml");

    // if (!file) return 1;
    // t_irealpro_song song = {0};
    // if (parse_musicxml_song(&song, STR_REF("P1"), file, file_len) != 0) {
	// 	printf("PARSER_STOP_ERROR\n");
    //     free(file);
    //     return 1;
    // }
    t_irealpro_song songs[2] = {0};
    char* file = open_file(&file_len, "musicxml/Misty.musicxml");
    if (parse_musicxml_song(songs, STR_REF("P1"), file, file_len) != 0) {
		printf("PARSER_STOP_ERROR\n");
        free(file);
        return 1;
    }
    free(file);
    file = open_file(&file_len, "musicxml/Out_of_Nothing.musicxml");
    if (parse_musicxml_song(songs + 1, STR_REF("P1"), file, file_len) != 0) {
		printf("PARSER_STOP_ERROR\n");
        free(file);
        return 1;
    }

	clock_gettime(CLOCK_MONOTONIC, &end);
	long sec = end.tv_sec - start.tv_sec;
	long nsec = end.tv_nsec - start.tv_nsec;
	double elapsed = sec + nsec / 1e9;

	// char *url = irp_get_song_html(&song);
	// if (url == NULL) {
	// 	printf("%s", "HTML RENDER FAIL");
	// 	return 4;
	// }
	char *url = irp_get_playlist_html("Wassup", songs, 2);
	printf("%s", "<h1>\n");
	printf("\tElapsed: %.9f sec\n", elapsed);
	printf("%s", "</h1>\n");
	printf("%s", "<h1>\n\t");
	printf("%s\n", url);
	printf("%s", "</h1>\n");
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

	// irp_song_free(&song);
	irp_song_free(songs);
	irp_song_free(songs + 1);
    free(file);
    return 0;
}
