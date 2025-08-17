#include "mxl_archive.h"
#include "parser.h"
#include <string.h>

t_mxl_archive	*mxl_archive_create()
{
	return calloc(1, sizeof(t_mxl_archive));
}

void	mxl_archive_free(t_mxl_archive	*mxl_archive)
{
	for (size_t i = 0; i < mxl_archive->files_count; i++) {
		free(mxl_archive->files[i].content.buf);
		free(mxl_archive->files[i].filename);
	}
	free(mxl_archive->files);
	free(mxl_archive);
}

void	mxl_archive_append_file(t_mxl_archive *mxl_archive, char *filename, char *file_buf, size_t file_len)
{
	if (mxl_archive->files_count == 0) {
		mxl_archive->files = malloc(sizeof(t_archive_file) * 4);
	} else if (mxl_archive->files_count > 3 && mxl_archive->files_count % 2 == 0) {
		t_archive_file	*tmp;
		tmp = realloc(mxl_archive->files, sizeof(t_archive_file) * mxl_archive->files_count * 2);
		if (tmp) mxl_archive->files = tmp;
	}
	if (!mxl_archive->files)
		return;
	mxl_archive->files[mxl_archive->files_count].content.buf = file_buf;
	mxl_archive->files[mxl_archive->files_count].content.len = file_len;
	mxl_archive->files[mxl_archive->files_count].content.cap = 1;
	mxl_archive->files[mxl_archive->files_count].filename = strdup(filename);
	mxl_archive->files_count += 1;
}

char	*mxl_archive_get_file_buf(t_mxl_archive *mxl_archive, size_t file_index)
{
	return mxl_archive->files[file_index].content.buf;
}

size_t	mxl_archive_get_file_len(t_mxl_archive *mxl_archive, size_t file_index)
{
	return mxl_archive->files[file_index].content.len;
}

size_t	mxl_archive_get_files_count(t_mxl_archive *mxl_archive)
{
	return mxl_archive->files_count;
}

int parse_container_xml(void *user_data, t_sax_context *context)
{
	char **musicxml_filename = (char **)user_data;
	static const da_str_ref rootfile = STR_REF("rootfile");

	if ((context->found.type == XML_SELF_CLOSING || context->found.type == XML_TAG_OPEN)
		&& str_ref_eq(&context->found.target, &rootfile)) {
		da_str_ref val = {0};
		if (sax_get_attrv(context, &val, "full-path") != 0)
			return PARSER_STOP_ERROR;
		*musicxml_filename = malloc(sizeof(char) * val.len + 1);
		if (*musicxml_filename == NULL)
			return PARSER_STOP_ERROR;
		memcpy(*musicxml_filename, val.buf, val.len);
		(*musicxml_filename)[val.len] = '\0';
		return PARSER_STOP;
	}
	return PARSER_CONTINUE;
}

int	mxl_archive_get_musicxml_index(t_mxl_archive *mxl_archive)
{
	char *musicxml_filename = NULL;
	int container_fileindex = -1;

	if (mxl_archive->files_count == 0)
		return -1;
	for (size_t i = 0; i < mxl_archive->files_count; ++i) {
		if (strcmp(mxl_archive->files[i].filename, "META-INF/container.xml") == 0)
			container_fileindex = i;
	}
	if (container_fileindex == -1)
		return -1;
    t_sax_scanner scanner = sax_scanner_init(
								mxl_archive->files[container_fileindex].content.buf,
								mxl_archive->files[container_fileindex].content.len);
    t_sax_context context = sax_context_init(&scanner);

	if (sax_parse_xml(parse_container_xml, &musicxml_filename, &context) != 0
		|| musicxml_filename == NULL) {
		if (musicxml_filename)
			free(musicxml_filename);
		return -1;
	}
	for (size_t i = 0; i < mxl_archive->files_count; ++i) {
		if (strcmp(mxl_archive->files[i].filename, musicxml_filename) == 0) {
			free(musicxml_filename);
			return i;
		}
	}
	free(musicxml_filename);
	return -1;
}

