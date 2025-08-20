#include "mxl2irp.h"

const char *get_error_code_str(ErrorEnum err)
{
	static const char *error_codes[] = {
		NULL,
		"ERROR_XML_FILE_CORRUPT",
		"ERROR_EMPTY_SCORE",
		"ERROR_ZOOM_FAILED",
		"ERROR_TOO_MUCH_CHORDS",
		"ERROR_TOO_MUCH_DEGREES",
		"ERROR_UNVALID_CHORD_KIND",
		"ERROR_UNVALID_KEY",
		"ERROR_UNVALID_DIVISIONS",
		"ERROR_UNVALID_TIME_SIGNATURE",
		NULL
	};
	return (err < 0 || err > ERROR_COUNT ?  NULL : error_codes[err]);
}


ErrorEnum mxl2irp_result_get_error_code(t_mxl2irp_result *result)
{
	return result->error_code;
}

void *mxl2irp_result_get_item(t_mxl2irp_result *result)
{
	return result->item;
}
char *mxl2irp_result_get_error_details(t_mxl2irp_result *result)
{
	return result->error_details;
}
