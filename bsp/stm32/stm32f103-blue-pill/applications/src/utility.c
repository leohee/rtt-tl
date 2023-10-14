
#include "inc_files.h"


const char Hex2Ascii[17] = "0123456789ABCDEF";

int hex_printf (const uint8_t *buff, int count)
{
	uint32_t i = 0, j = 0;

	char str_val[75] = {0};
	uint32_t index = 0;
	uint32_t str_index = 0;
	uint32_t cnt = (count+15)>>4;

	if (count < 0) {
		return -1;
	}

	if (cnt > 16) {
		cnt = 16;
		printf("!!! TOO LONG. SHOW HEAD 256 bytes !!!\n");
	}

	for (i = 0; i < cnt; i++) {
		index = 0;
		str_index = 0;

		for (j = 0; j < 16; j++) {
			if (j + (i << 4) < count) {
				str_val[index++] = Hex2Ascii[(buff[j + (i << 4)] & 0xF0) >> 4];
				str_val[index++] = Hex2Ascii[buff[j + (i << 4)] & 0xF];
				str_val[index++] = ' ';
			} else {
				str_val[index++] = ' ';
				str_val[index++] = ' ';
				str_val[index++] = ' ';
			}
		}

		index += str_index;
		str_val[index++] = '\n';
		str_val[index++] = '\0';
		printf("%s", str_val);
	}

	return 0;
}


