/** !
 * High level abstractions for thread pool 
 * 
 * @file parallel/threadpool.h
 *
 * @author Jacob Smith
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

// queue submodule
#include <queue/queue.h>

// parallel
#include <parallel/parallel.h>
#include <parallel/thread.h>

// Forward declarations
struct thread_pool_s;

// Type definitions
typedef struct thread_pool_s thread_pool;

// Function declarations

// Constructors
/** !
 * Construct a thread pool
 * 
 * @param pp_thread_pool  result
 * @param thread_quantity the quantity of threads
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_pool_construct ( thread_pool **pp_thread_pool, int thread_quantity );

/** !
 * Execute a job on a thread pool
 * 
 * @param p_thread_pool     the thread pool
 * @param pfn_parallel_task pointer to job function
 * @param p_parameter       the parameter of the parallel task
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_pool_execute ( thread_pool *p_thread_pool, fn_parallel_task *pfn_parallel_task, void *p_parameter );

/** !
 * Test if the thread pool is idle
 * 
 * @param p_thread_pool the thread pool
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT bool thread_pool_is_idle ( thread_pool *p_thread_pool );

/** !
 * Destroy a thread pool
 * 
 * @param pp_thread_pool pointer to thread pool pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int thread_pool_destroy ( thread_pool **pp_thread_pool );
