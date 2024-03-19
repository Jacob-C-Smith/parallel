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

#define BUILD_SYNC_WITH_MONITOR

// sync submodule
#include <sync/sync.h>

// queue submodule
#include <queue/queue.h>

// parallel
#include <parallel/parallel.h>
#include <parallel/thread.h>

// Preprocessor definitions
#define PARALLEL_THREAD_POOL_MAX_THREADS 64

// structure definitions
struct thread_pool_s
{
    int thread_quantity,
        running_jobs,
        pending_jobs,
        complete_jobs;
    bool running;
    queue *jobs;
    monitor _monitor;
    mutex _lock;
    parallel_thread *p_main, 
                    *_p_threads[PARALLEL_THREAD_POOL_MAX_THREADS];
};

/** !
 * Allocate memory for a thread pool
 * 
 * @param pp_thread_pool return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_pool_create ( thread_pool **pp_thread_pool );

DLLEXPORT int thread_pool_construct ( thread_pool **pp_thread_pool, int thread_quantity );

DLLEXPORT int thread_pool_run ( thread_pool *p_thread_pool, void *job );

DLLEXPORT int thread_pool_pending_jobs ( thread_pool *p_thread_pool, int *p_result );

DLLEXPORT int thread_pool_complete_jobs ( thread_pool *p_thread_pool, void *p_result );

DLLEXPORT bool thread_pool_is_idle ( thread_pool *p_thread_pool );

/** !
 * Destroy a thread pool
 * 
 * @param pp_thread_pool pointer to thread pool pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int thread_pool_destroy ( thread_pool **pp_thread_pool );