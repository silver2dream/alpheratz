#ifndef __ALPHA_MACRO_H__
#define __ALPHA_MACRO_H__

#define ALPHA_MAX_TITLE_STR 1000

#define ALPHA_MAX_ERROR_STR 2048

// The maximum 32-bit unsigned integer.
#define ALPHA_MAX_UINT32_VALUE (uint32_t)0xffffffff

#define ALPHA_INT64_LEN (sizeof("-9223372036854775808") - 1)

#define Alpha_Cpy_Mem(dst, src, n) (((u_char *)memcpy(dst, src, n)) + (n))

#define ALPHA_ERROR_LOG_PATH "logs/error.log"

#endif