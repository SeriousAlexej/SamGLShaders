/* #undef HAVE_ENDIAN_H */
#define HAVE_FCNTL_H 1
/* #undef HAVE_SCHED_H */
/* #undef HAVE_UNISTD_H */
/* #undef HAVE_SYS_PARAM_H */
#define HAVE_SYS_STAT_H 1
/* #undef HAVE_SYS_TIME_H */
#define HAVE_SYS_TYPES_H 1
/* #undef HAVE_STDINT_H */

#define HAVE_CLOSE 1
#define HAVE_GETPID 1
/* #undef HAVE_GETTIMEOFDAY */
#define HAVE_OPEN 1
#define HAVE_READ 1
/* #undef HAVE_SCHED_YIELD */

/* #undef HAVE_SYNC_BUILTINS */
/* #undef HAVE_ATOMIC_BUILTINS */

#define HAVE_LOCALE_H 1
#define HAVE_SETLOCALE 1

/* #undef HAVE_INT32_T */
#ifndef HAVE_INT32_T
#  define int32_t int
#endif

/* #undef HAVE_UINT32_T */
#ifndef HAVE_UINT32_T
#  define uint32_t unsigned int
#endif

/* #undef HAVE_UINT16_T */
#ifndef HAVE_UINT16_T
#  define uint16_t unsigned short
#endif

/* #undef HAVE_UINT8_T */
#ifndef HAVE_UINT8_T
#  define uint8_t unsigned char
#endif

/* #undef HAVE_SSIZE_T */
#ifndef HAVE_SSIZE_T
#  define ssize_t int
#endif

#define USE_URANDOM 1
/* #undef USE_WINDOWS_CRYPTOAPI */

#define INITIAL_HASHTABLE_ORDER 3
