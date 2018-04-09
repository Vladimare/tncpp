#ifndef _COMPILERSPEC_H_
#define _COMPILERSPEC_H_


#if defined(__ARMCC_VERSION)
# define COMPILE_ALIGN(x) __attribute__((aligned(x)))
#elif defined(_WIN32)
# define COMPILE_ALIGN(x) __declspec(align(x))
#endif

#if defined(_WIN32)
#define snprintf _snprintf
#endif


#endif /*_COMPILERSPEC_H_*/
