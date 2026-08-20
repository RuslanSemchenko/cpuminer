/*
 * Jansson public configuration for the cpuminer in-tree (compat) build.
 * Based on the cmake-generated jansson_config.h from Jansson 2.14.
 */
#ifndef JANSSON_CONFIG_H
#define JANSSON_CONFIG_H

/* All supported platforms have stdint.h */
#define HAVE_STDINT_H 1

/* Include our standard type header for the integer typedef */
#if defined(HAVE_STDINT_H)
#  include <stdint.h>
#elif defined(HAVE_INTTYPES_H)
#  include <inttypes.h>
#elif defined(HAVE_SYS_TYPES_H)
#  include <sys/types.h>
#endif

/*
 * JSON_INTEGER_IS_LONG_LONG: use long long as the JSON integer type.
 * sizeof(long long) == 8 on all supported platforms (LP64 on Unix,
 * LLP64 on Windows).
 */
#define JSON_INTEGER_IS_LONG_LONG 1

/*
 * json_int_t, JSON_INTEGER_FORMAT: defined by jansson.h itself once
 * JSON_INTEGER_IS_LONG_LONG is set.
 */

/* JSON_INLINE */
#ifdef __cplusplus
#define JSON_INLINE inline
#else
#define JSON_INLINE inline
#endif

/* localeconv() is available on all supported platforms */
#define JSON_HAVE_LOCALECONV 1

/* __atomic builtins available with modern GCC/Clang/MSVC 2019+ */
#define JSON_HAVE_ATOMIC_BUILTINS 1

/*
 * Maximum recursion depth for parsing JSON input.
 * This limits the depth of e.g. array-within-array constructions.
 */
#define JSON_PARSER_MAX_DEPTH 2048

#endif /* JANSSON_CONFIG_H */
