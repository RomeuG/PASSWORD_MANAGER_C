#ifndef __BASE64_H_
#define __BASE64_H_

#include <stdlib.h>
#include "defines.h"

int _b64_encode(const unsigned char* buffer, size_t length, char** b64text);
int _b64_decode(char* b64message, unsigned char** buffer);

#endif
