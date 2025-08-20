#ifndef MXL_ARCHIVE_H
#define MXL_ARCHIVE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define WASM_EXPORT EMSCRIPTEN_KEEPALIVE
#else
	#define WASM_EXPORT 
#endif

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

WASM_EXPORT	t_mxl_archive	*mxl_archive_create();
WASM_EXPORT	void			mxl_archive_free(t_mxl_archive	*mxl_archive);
WASM_EXPORT	void			mxl_archive_append_file(t_mxl_archive *mxl_archive, char *filename,
												char *file_buf, size_t file_len);
WASM_EXPORT	int				mxl_archive_get_musicxml_index(t_mxl_archive *mxl_archive);
WASM_EXPORT	char			*mxl_archive_get_file_buf(t_mxl_archive *mxl_archive, size_t file_index);
WASM_EXPORT	size_t			mxl_archive_get_file_len(t_mxl_archive *mxl_archive, size_t file_index);
WASM_EXPORT	size_t			mxl_archive_get_files_count(t_mxl_archive *mxl_archive);

#ifdef __cplusplus
}
#endif

#endif // MXL_ARCHIVE_H
