#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include "base64.h"

s8* _b64_encode(const u8 *src, size_t len)
{
	int i = 0;
	int j = 0;

	s8 *enc = NULL;
	size_t size = 0;

	u8 buf[4];
	u8 tmp[3];

	enc = malloc(0);
	if (NULL == enc) { return NULL; }

	while (len--) {
		tmp[i++] = *(src++);

		if (i == 3) {
	  		buf[0] = (tmp[0] & 0xfc) >> 2;
	  		buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
	  		buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
	  		buf[3] = tmp[2] & 0x3f;

	  		enc = realloc(enc, size + 4);
	  		for (i = 0; i < 4; ++i) {
				enc[size++] = b64_table[buf[i]];
	  		}

	  		i = 0;
		}
	}

  	if (i > 0) {
		for (j = i; j < 3; ++j) {
	  		tmp[j] = '\0';
		}

		buf[0] = (tmp[0] & 0xfc) >> 2;
		buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
		buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
		buf[3] = tmp[2] & 0x3f;

		for (j = 0; (j < i + 1); ++j) {
	  		enc = realloc(enc, size + 1);
	  		enc[size++] = b64_table[buf[j]];
		}

		while ((i++ < 3)) {
	  		enc = realloc(enc, size + 1);
	  		enc[size++] = '=';
		}
  	}

  	enc = realloc(enc, size + 1);
  	enc[size] = '\0';

  	return enc;
}

u8* _b64_decode_ex(const s8 *src, size_t len, size_t *decsize)
{
  	int i = 0;
  	int j = 0;
  	int l = 0;

  	size_t size = 0;

  	u8 *dec = NULL;
  	u8 buf[3];
  	u8 tmp[4];

  	dec = malloc(0);
  	if (NULL == dec) {
		return NULL;
	}

  	while (len--) {
		if ('=' == src[j]) { break; }
		if (!(isalnum(src[j]) || '+' == src[j] || '/' == src[j])) { break; }

		tmp[i++] = src[j++];

		if (4 == i) {
	  		for (i = 0; i < 4; ++i) {
				for (l = 0; l < 64; ++l) {
			  		if (tmp[i] == b64_table[l]) {
						tmp[i] = l;
						break;
		  			}
				}
	  		}

	  		buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
	  		buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
	  		buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

	  		dec = (unsigned char *) realloc(dec, size + 3);
	  		for (i = 0; i < 3; ++i) {
				dec[size++] = buf[i];
	  		}

	  		i = 0;
		}
  	}

  	if (i > 0) {
		for (j = i; j < 4; ++j) {
	  		tmp[j] = '\0';
		}

		for (j = 0; j < 4; ++j) {
			for (l = 0; l < 64; ++l) {
		  		if (tmp[j] == b64_table[l]) {
					tmp[j] = l;
					break;
		  		}
			}
		}

		buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
		buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
		buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];

		dec = realloc(dec, size + (i - 1));
		for (j = 0; (j < i - 1); ++j) {
	  		dec[size++] = buf[j];
		}
  	}

  	dec = realloc(dec, size + 1);
  	dec[size] = '\0';

  	if (decsize != NULL) *decsize = size;

  	return dec;
}

u8* _b64_decode(const s8 *src, size_t len)
{
	return _b64_decode_ex(src, len, NULL);
}
