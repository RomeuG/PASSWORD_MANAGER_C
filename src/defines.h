#ifndef __DEFINES_H_
#define __DEFINES_H_

#include <inttypes.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

// encryption
#define AES_KEY_LENGTH 64

#define AES_128_CBC "aes-128-cbc"
#define AES_256_CBC "aes-256-cbc"
#define HASH_SHA1 "sha1"

#define PBKDF2_OUTPUT_SIZE 32
#define PBKDF2_ITERATIONS 1000

#define EVP_SHA1 EVP_sha1()
#define EVP_SHA224 EVP_sha224()
#define EVP_SHA256 EVP_sha256()
#define EVP_SHA384 EVP_sha384()
#define EVP_SHA512 EVP_sha512()

#define AES_SALT_LEN_8 8
#define AES_SALT_LEN_16 16

// comparators for macros
#define CMP_E ==
#define CMP_NE !=
#define CMP_G >
#define CMP_GE >=
#define CMP_L <
#define CMP_LE <=

// comparator for macro strings
#define CMP_E_STR "=="
#define CMP_NE_STR "!="
#define CMP_G_STR ">"
#define CMP_GE_STR ">="
#define CMP_L_STR "<"
#define CMP_LE_STR "<="

#endif