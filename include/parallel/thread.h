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

// parallel
#include <parallel/parallel.h>

// structure definitions
struct parallel_thread_s
{

    // Platform dependent struct members
    #ifdef _WIN64
        // TODO
    #else
        pthread_t platform_dependent_thread;
    #endif
};

// Constructors
/** !
 *  Allocate a parallel thread
 *
 * @param p_thread : return
 * 
 * @sa parallel_thread_start
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_thread_create ( parallel_thread **pp_parallel_thread );

/** !
 * Start a new parallel thread
 * 
 * @param pp_parallel_thread   return
 * @param pfn_function_pointer pointer to start function
 * @param p_parameter          parameter for start function
 * 
 * @sa parallel_thread_join
 * @sa parallel_thread_destroy
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_thread_start ( parallel_thread **pp_parallel_thread, void *pfn_function_pointer, void *p_parameter );

/** !
 *  Wait for a thread to end, then destory it
 *
 * @param pp_parallel_thread pointer to a parallel thread pointer
 *
 * @sa parallel_thread_create
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_thread_join ( parallel_thread **pp_parallel_thread );

/** !
 *  Destroy a thread and all its allocations
 *
 * @param p_thread pointer to a thread
 *
 * @sa parallel_thread_create
 *
 * @return 1 on success, 0 on error
 */
//DLLEXPORT int parallel_thread_detach ( parallel_thread *p_thread );

/** !
 *  Destroy a thread
 *
 * @param pp_parallel_thread : Pointer to a parallel thread pointer
 *
 * @sa parallel_thread_create
 *
 * @return 1 on success, 0 on error
 */
int parallel_thread_destory ( parallel_thread **pp_parallel_thread );
