#ifndef MXL_ARCHIVE_H
#define MXL_ARCHIVE_H

#include "da.h"

typedef struct s_archive_file
{
	da_str		content;
	char		*filename;
} t_archive_file;

typedef struct s_mxl_archive
{
	t_archive_file	*files;
	size_t		files_count;
} t_mxl_archive;

t_mxl_archive	*mxl_archive_create();
void			mxl_archive_free(t_mxl_archive	*mxl_archive);
void			mxl_archive_append_file(t_mxl_archive *mxl_archive, char *filename,
											char *file_buf, size_t file_len);
int				mxl_archive_get_musicxml_index(t_mxl_archive *mxl_archive);
char			*mxl_archive_get_file_buf(t_mxl_archive *mxl_archive, size_t file_index);
size_t			mxl_archive_get_file_len(t_mxl_archive *mxl_archive, size_t file_index);
size_t			mxl_archive_get_files_count(t_mxl_archive *mxl_archive);

#endif // MXL_ARCHIVE_H
