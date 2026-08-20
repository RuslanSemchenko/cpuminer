/*
 * Jansson private configuration for the cpuminer in-tree (compat) build.
 * Matches the configuration that the full autotools/cmake build would
 * generate on all supported platforms (Linux, macOS, MinGW).
 */
#ifndef JANSSON_PRIVATE_CONFIG_H
#define JANSSON_PRIVATE_CONFIG_H

/* All supported platforms have stdint.h */
#define HAVE_STDINT_H 1

/* snprintf is always available on supported platforms */
#define HAVE_SNPRINTF 1

/* localeconv() is available on all supported platforms */
#define JSON_HAVE_LOCALECONV 1

/* __atomic / __sync builtins are available with GCC >= 4.7 and Clang */
#define JSON_HAVE_ATOMIC_BUILTINS 1

/* 64-bit integer support */
#define HAVE_INT64_T 1

/* ssize_t available via sys/types.h on all platforms */
#define HAVE_SSIZE_T 1

/*
 * Standard headers available on all supported platforms. endian.h is
 * GNU libc specific (Linux); on macOS/FreeBSD endianness is defined via
 * <sys/param.h> instead, so define HAVE_ENDIAN_H only on Linux.
 */
#define HAVE_INTTYPES_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIME_H 1
#if defined(__linux__)
#define HAVE_ENDIAN_H 1
#endif
/* unistd/fcntl/sched exist on POSIX only (not on native Windows/MinGW) */
#if !defined(_WIN32)
#define HAVE_UNISTD_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SCHED_H 1
#endif

/*
 * Random seed generation (hashtable_seed.c).
 * On POSIX, seed comes from /dev/urandom; on native Windows, jansson
 * falls back to the CryptoAPI (USE_WINDOWS_CRYPTOAPI handled in-code).
 */
#define USE_URANDOM 1
#if !defined(_WIN32)
#define HAVE_OPEN 1
#define HAVE_CLOSE 1
#define HAVE_READ 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_GETPID 1
#define HAVE_SCHED_YIELD 1
#endif
#define HAVE_SYNC_BUILTINS 1

#endif /* JANSSON_PRIVATE_CONFIG_H */
