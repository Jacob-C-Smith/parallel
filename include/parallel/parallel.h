/** !
 * @file parallel/parallel.h
 * 
 * @author Jacob C Smith
 *
 * High level abstractions for parallelized computing
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// sync submodule
#include <sync/sync.h>

// log submodule
#include <log/log.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Set the reallocator for the dict submodule
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array submodule
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the json submodule
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef PARALLEL_REALLOC
    #define PARALLEL_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct parallel_thread_s;
struct thread_pool_s;
struct schedule_s;

// Type definitions
typedef struct parallel_thread_s parallel_thread;
typedef struct thread_pool_s thread_pool;
typedef struct schedule_s schedule;

// Initializers
/** !
 *  Initialize parallel 
 * 
 * @param void
 * 
 * @return 1 on success, 0 on error 
 */ 
int parallel_init ( void );