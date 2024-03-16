/** !
 * @file parallel/threadpool.h
 * 
 * @author Jacob C Smith
 *
 * High level abstractions for thread pool 
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

// structure definitions
struct thread_pool_s
{
    int thread_quantity;
    parallel_thread **pp_threads;
    bool *working;
    queue *jobs;
    bool running;
    int (*pfn_task)(void *job);
    mutex _lock;
};

/** !
 * Allocate memory for a thread pool
 * 
 * @param pp_thread_pool return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_pool_create ( thread_pool **pp_thread_pool );

DLLEXPORT int thread_pool_construct ( thread_pool **pp_thread_pool, int thread_quantity, int (*pfn_task)(void *job) );

DLLEXPORT int thread_pool_enqueue ( thread_pool *p_thread_pool, void *job );

/** !
 * Destroy a thread pool
 * 
 * @param pp_thread_pool pointer to thread pool pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int thread_pool_destroy ( thread_pool **pp_thread_pool );