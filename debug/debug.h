#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "defines.h"

#include <stddef.h>		// size_t
#include <stdio.h>		// fprintf

//
// DEBUG BREAK
//
#if   defined (_MSC_VER) || defined (__INTEL_COMPILER)
	#include <intrin.h>		// __debugbreak
	#define DEBUG_BREAK __debugbreak()
#elif defined (__ARMCC_VERSION)
	#define DEBUG_BREAK __breakpoint(42)
#elif defined (__GNUC__)
	#define DEBUG_BREAK __builtin_trap()
#else
	#include <stdlib.h>		// abort
	#define DEBUG_BREAK abort()	
#endif

//
// CURRENT FUNCTION NAME
//
#if defined(__GNUC__) || defined(__clang__) 
	#define __function__ __PRETTY_FUNCTION__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define __function__ __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define __function__ __func__
#elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)
	#define __function__ __FUNCTION__
#else
	#define __function__ "<unknown>"
#endif

//
// ASSERTION
//
#ifdef ASSERTION_ENABLE
	#define __ASSERT__(expr, mes) \
		if((expr)) {} \
		else \
		{ \
			fprintf(stdout, "[ASSERTION FAILED]:\n>file: %s\n>function: '%s'\n>line: %d\n>expression: %s\n>message: %s\n",\
			 __FILE__, __function__, __LINE__, #expr, mes); DEBUG_BREAK; \
		}
	#define   M_ASSERT(expr, mes) __ASSERT__(expr, mes)
	#define		ASSERT(expr)		__ASSERT__(expr, "-")
#else
	#define __ASSERT__(expr, mes)
	#define   M_ASSERT(expr, mes)
	#define		ASSERT(expr)
#endif	// ASSERTION_ENABLE

//
// MEMORY DUMP
//
#if defined(_WIN64) || defined(__x86_64__)
	#define DEBUG_MEMORY_ALIGNMENT 8
	#define DEBUG_MEMORY_ROW_LENGTH 8
#else
	#define DEBUG_MEMORY_ALIGNMENT 4
	#define DEBUG_MEMORY_ROW_LENGTH 4
#endif

// prints some piece of memory
extern void show_memory(const void *start, const size_t size);

//
// OUTPUT FORMATS
//
#if defined(_WIN64) || defined(__x86_64__)
	// 64-bit systems
	#define HEX_FORMAT	"0x%08lX"
	#define UINT_FORMAT "%lu"
#else
	// 32-bit systems
	#define HEX_FORMAT	"0x%08X"
	#define UINT_FORMAT	"%u"
#endif
//
// MESSAGES
//
#ifdef DEBUG_MESSAGES
	#define PRINT(mes) fprintf(stdout, "[DEBUG MESSAGE]: %s\n", mes)
#define PRINT_UINT(value) fprintf(stdout, "[DEBUG_MESSAGE]: %s = " UINT_FORMAT "\n", #value, (const size_t)value)
#define PRINT_HEX(value) fprintf(stdout, "[DEBUG_MESSAGE]: %s = " HEX_FORMAT "\n", #value, (const size_t)value)
#else
	#define PRINT(mes)
	#define PRINT_UINT(value)
	#define PRINT_HEX(value)
#endif	// DEBUG_MESSAGES

//
// FILLING MEMORY WITH ZEROS
// only helpful when show_memory function is used :D
//
#ifdef INIT_WITH_ZERO
	#include <string.h>			// memset
	#define MEMSET_ZERO(ptr, size) { PRINT("MEMORY CLEARED"); memset((ptr), 0, (size)); }
#else
	#define MEMSET_ZERO(ptr, size)
#endif	// INIT_WITH_ZERO
#endif	// _DEBUG_H_
