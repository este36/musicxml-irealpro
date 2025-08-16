#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "parser.h"

t_irealpro_song *get_song_from_path(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }

    fseek(f, 0, SEEK_END);

    long file_len = ftell(f);
    rewind(f);

    char *file_buf = malloc(file_len + 1); 
    if (!file_buf) {
        fclose(f);
        return NULL;
    }

    fread(file_buf, 1, file_len, f);
    file_buf[file_len] = '\0';
    fclose(f);

	t_irealpro_song *irp_song = parse_musicxml(file_buf, file_len);
	free(file_buf);
	return irp_song;
}

void	print_url(const char *url)
{
	printf("%s", "<h1>");
	printf("%s", url);
	printf("%s", "</h1>\n");
}

int main(int argc, char **argv) {
	if (argc == 1) {
		fprintf(stderr, "Usage: %s (1 or more)[path-to-musicxml]\n", argv[0]);
		return 1;
	}

	if (argc == 2) {
		
		t_irealpro_song *irp_song = get_song_from_path(argv[1]);
		if (irp_song == NULL) {
			fprintf(stderr, "%s: PARSE SONG FAIL\n", argv[1]);
			return 1;
		}
		char *url = irp_song_get_html(irp_song);
		if (url == NULL) {
			fprintf(stderr, "HTML RENDER FAIL\n");
			return 1;
		}
		print_url(url);
		irp_song_free(irp_song);
		free(url);
	} else {
		t_irealpro_playlist *playlist = irp_playlist_create("Test");

		for (int i = 1; i < argc; i++) {
			t_irealpro_song *irp_song = get_song_from_path(argv[i]);
			if (irp_song == NULL) {
				irp_playlist_free(playlist);
				fprintf(stderr, "%s: PARSE SONG FAIL\n", argv[i]);
				return 1;
			}
			irp_playlist_append(playlist, irp_song);
		}
		char *url = irp_playlist_get_html(playlist);
		if (url == NULL) {
			fprintf(stderr, "HTML RENDER FAIL\n");
			return 1;
		}
		print_url(url);
		irp_playlist_free(playlist);
		free(url);
	}
    return 0;
}
