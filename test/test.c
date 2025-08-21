#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include "miniz.h"
#pragma GCC diagnostic pop
#include "mxl2irp.h"
#include "mxl_archive.h"

int is_mxl_file(const char *f)
{
	const size_t len = strlen(f);
	return (len > 4
			&& f[len - 4] == '.'
			&& f[len - 3] == 'm'
			&& f[len - 2] == 'x'
			&& f[len - 1] == 'l');
}

t_mxl_archive	*get_mxl_archive_from_mxl_file(const char *file_buf, size_t file_len)
{
	t_mxl_archive *mxl_archive = mxl_archive_create();
	if (!mxl_archive)
		return NULL;
	mz_zip_archive zip = {0};
	if (!mz_zip_reader_init_mem(&zip, file_buf, file_len, 0))
		return NULL;
	size_t num_files = (size_t)mz_zip_reader_get_num_files(&zip);
	for (size_t i = 0; i < num_files; i++) {
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip, i, &file_stat))
			continue;
		size_t uncomp_len = 0;
		char *uncomp_buf = (char *)mz_zip_reader_extract_to_heap(&zip, i, &uncomp_len, 0);
		if (uncomp_buf) {
			mxl_archive_append_file(mxl_archive, file_stat.m_filename, uncomp_buf, uncomp_len);
		}
	}
	mz_zip_reader_end(&zip);
	return mxl_archive;
}

t_irealpro_song	*get_song_from_path(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;
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

	t_mxl2irp_result *result;
	if (is_mxl_file(path)) {
		t_mxl_archive *mxl_archive = get_mxl_archive_from_mxl_file(file_buf, file_len);
		if (!mxl_archive) {
			free(file_buf);
			return NULL;
		}
		int musicxml_file_index = mxl_archive_get_musicxml_index(mxl_archive);
		if (musicxml_file_index < 0) {
			free(file_buf);
			mxl_archive_free(mxl_archive);
			return NULL;
		}
		char *musicxml_buf = mxl_archive_get_file_buf(mxl_archive, musicxml_file_index);
		size_t musicxml_len = mxl_archive_get_file_len(mxl_archive, musicxml_file_index);
		result = parse_musicxml(musicxml_buf, musicxml_len);
		mxl_archive_free(mxl_archive);
	} else {
		result = parse_musicxml(file_buf, file_len);
	}
	free(file_buf);

	if (result->error_code != ERROR_UNSET) {
		fprintf(stderr, "Error: %s: %s\n", path, get_error_code_str(result->error_code));
		if (result->error_details) {
			fprintf(stderr, "Details: %s\n", result->error_details);
			free(result->error_details);
		}
		free(result);
		return NULL;
	}
	t_irealpro_song	*song = result->item;
	free(result);
	return song;
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
		if (!irp_song)
			return 1;
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
			if (!irp_song) {
				irp_playlist_free(playlist);
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
