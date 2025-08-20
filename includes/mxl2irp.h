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

typedef struct s_mxl2irp_result
{
	void		*item;
	ErrorEnum	error_code;
	char		*error_details;
} t_mxl2irp_result

#include "parser.h"
#include "irealpro.h"

#ifdef __cplusplus
}
#endif

#endif // MXL2IRP_H
