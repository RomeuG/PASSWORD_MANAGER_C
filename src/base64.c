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

// ------------------------------------------
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <assert.h>

size_t calcDecodeLength(const char* b64input) { //Calculates the length of a decoded string
	size_t len = strlen(b64input),
		padding = 0;

	if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
		padding = 2;
	else if (b64input[len-1] == '=') //last char is =
		padding = 1;

	return (len*3)/4 - padding;
}

int Base64Decode(char* b64message, unsigned char** buffer, size_t* length) { //Decodes a base64 encoded string
	BIO *bio, *b64;

	int decodeLen = calcDecodeLength(b64message);
	*buffer = (unsigned char*)malloc(decodeLen + 1);
	(*buffer)[decodeLen] = '\0';

	bio = BIO_new_mem_buf(b64message, -1);
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer
	*length = BIO_read(bio, *buffer, strlen(b64message));
	assert(*length == decodeLen); //length should equal decodeLen, else something went horribly wrong
	BIO_free_all(bio);

	return (0); //success
}

int Base64Encode(const unsigned char* buffer, size_t length, char** b64text) { //Encodes a binary safe base 64 string
	BIO *bio, *b64;
	BUF_MEM *bufferPtr;

	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);

	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
	BIO_write(bio, buffer, length);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bufferPtr);
	BIO_set_close(bio, BIO_NOCLOSE);
	BIO_free_all(bio);

	*b64text=(*bufferPtr).data;

	return (0); //success
}
