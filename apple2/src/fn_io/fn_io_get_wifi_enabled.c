#include <stdint.h>
#include "fujinet-io.h"
#include "fujinet-lib-apple2.h"

bool fn_io_get_wifi_enabled(void)
{
	sp_error = sp_get_fuji_id();
	if (sp_error <= 0) {
		return false;
	}

	sp_error = sp_status(sp_fuji_id, 0xEA);
	if (sp_error == 0) {
		return sp_payload[0] != 0;
	}
	return false;
}
