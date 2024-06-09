#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-cbm.h"

extern uint16_t ak_creator_id;
extern uint8_t ak_app_id;
extern enum AppKeySize ak_appkey_size;

bool fuji_write_appkey(uint8_t key_id, uint16_t count, uint8_t *data)
{
	bool ret;
	uint8_t err = 0;
	uint8_t mode = 0;
	uint8_t pl[7];
	uint8_t *out_data;
	uint16_t data_size = count + 1; // cmd (1 byte)  + data length

	if (ak_creator_id == 0) {
		return false;
	}

	pl[0] = FUJICMD_OPEN_APPKEY;
	pl[1] = ak_creator_id & 0xFF;
	pl[2] = ak_creator_id >> 8;
	pl[3] = ak_app_id;
	pl[4] = key_id;
	pl[5] = 0x01; 				// WRITE mode
	pl[6] = 0;					// reserved

	// send the creator / app / mode values
	if (!open_close(7, pl)) {
		// something went wrong, abort
		return false;
	}

	out_data = malloc(data_size);
	memset(out_data, 0, data_size);

	// now do a write of the key data, on IEC we don't need to send the count, as the write doesn't have to be a fixed size
	out_data[0] = FUJICMD_WRITE_APPKEY;
	memcpy(&out_data[1], data, count);

	ret = open_close(data_size, out_data);
	free(out_data);
	return ret;

}
