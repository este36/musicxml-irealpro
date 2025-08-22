#ifndef MXL2IRP_H
#define MXL2IRP_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>
	#define WASM_EXPORT EMSCRIPTEN_KEEPALIVE
#else
	#define WASM_EXPORT 
#endif

#include <stdlib.h>

typedef enum
{
	ERROR_UNSET,
	ERROR_XML_FILE_CORRUPT,
	ERROR_EMPTY_SCORE,
	ERROR_ZOOM_FAILED,
	ERROR_TOO_MUCH_CHORDS,
	ERROR_TOO_MUCH_DEGREES,
	ERROR_UNVALID_CHORD_KIND,
	ERROR_UNVALID_KEY,
	ERROR_UNVALID_DIVISIONS,
	ERROR_UNVALID_TIME_SIGNATURE,
	ERROR_CREDENTIALS_OVERFLOW,
	ERROR_COUNT
}	ErrorEnum;

typedef struct s_mxl2irp_result
{
	void		*item;
	ErrorEnum	error_code;
	char		*error_details;
}	t_mxl2irp_result;

WASM_EXPORT	const char	*get_error_code_str(ErrorEnum err);
WASM_EXPORT ErrorEnum	mxl2irp_result_get_error_code(t_mxl2irp_result *result);
WASM_EXPORT void		*mxl2irp_result_get_item(t_mxl2irp_result *result);
WASM_EXPORT	char		*mxl2irp_result_get_error_details(t_mxl2irp_result *result);

#include "parser.h"
#include "irealpro.h"

#ifdef __cplusplus
}
#endif

#endif // MXL2IRP_H
