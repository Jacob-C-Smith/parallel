/** !
 * @file thread_pool.c
 * 
 * @author Jacob Smith
 *
 * Thread pool implementation
 */

// parallel
#include <parallel/thread_pool.h>

// Function declarations
/** !
 * Wait for a job, then do the job
 * 
 * @param p_thread_pool the thread pool
 * 
 * @return 1 on success, 0 on error
 */
int thread_pool_work ( thread_pool *p_thread_pool )
{

    // Initialized data
    void *job = (void *) 0;

    // Wait for start
    while (p_thread_pool->running == false)
    {
        ;
    }
    

    // Wait for a job
    while ( p_thread_pool->running )
    {

        try_again:
        // Spin if there is nothing to do
        if ( queue_dequeue(p_thread_pool->jobs, &job) == 0 )
        {
            goto try_again;
        }

        // Do the job
        p_thread_pool->pfn_task(job);
    }

    // Success
    return 1;
}

int thread_pool_create ( thread_pool **pp_thread_pool )
{

    // Argument check
    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    thread_pool *p_thread_pool = PARALLEL_REALLOC(0, sizeof(thread_pool));

    // Error check
    if ( p_thread_pool == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_thread_pool, 0, sizeof(thread_pool));

    // Return a pointer to the caller
    *pp_thread_pool = p_thread_pool;

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int thread_pool_construct ( thread_pool **pp_thread_pool, int thread_quantity, int (*pfn_task)(void *job) )
{
    
    // Argument check
    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    thread_pool *p_thread_pool = (void *) 0,
                  _thread_pool = { 0 };
    parallel_thread **pp_threads = PARALLEL_REALLOC(0, sizeof(parallel_thread *) * thread_quantity);
    //bool *working = PARALLEL_REALLOC(0, sizeof(bool) * thread_quantity);
    queue *jobs = (void *) 0;

    // Error check
    //if ( working    == (void *) 0 ) goto no_mem;
    if ( pp_threads == (void *) 0 ) goto no_mem;

    // Allocate memory for a thread pool
    if ( thread_pool_create(&p_thread_pool) == 0 ) goto failed_to_allocate_thread_pool;

    // Allocate a queue
    if ( queue_construct(&jobs) == (void *) 0 ) goto failed_to_construct_queue;

    // Populate the thread pool struct
    _thread_pool = (thread_pool)
    {
        .thread_quantity = thread_quantity,
        .pp_threads      = pp_threads,
        .pfn_task        = pfn_task,
        .jobs            = jobs,
        .running         = false,
        ._lock           = 0
    };

    // Copy the thread pool struct to the heap
    memcpy(p_thread_pool, &_thread_pool, sizeof(thread_pool));

    // Create a lock
    mutex_create(&p_thread_pool->_lock);

    // Construct thread_quantity threads
    for (size_t i = 0; i < thread_quantity; i++) parallel_thread_start(&pp_threads[i], thread_pool_work, p_thread_pool);

    // Return a pointer to the caller
    *pp_thread_pool = p_thread_pool;

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // parallel errors
        {
            failed_to_allocate_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Failed to allocate thread pool in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // queue errors
        {
            failed_to_construct_queue:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Failed to construct jobs queue in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int thread_pool_enqueue ( thread_pool *p_thread_pool, void *job )
{
    
    // Argument check
    if ( p_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Add the job to the queue
    if ( queue_enqueue(p_thread_pool->jobs, job) == 0 ) goto failed_to_enqueue_task;

    // Success
    return 1;

    // Error handling
    {
            
        // Argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"p_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
        // queue errors
        {
            failed_to_enqueue_task:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Failed to enqueue job in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int thread_pool_destroy ( thread_pool **pp_thread_pool )
{

    // Argument check
    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    thread_pool *p_thread_pool = *pp_thread_pool;

    // No more pointer for caller
    *pp_thread_pool = (void *) 0;

    // Error check
    if ( p_thread_pool == (void *) 0 ) goto pointer_to_null_pointer;

    // TODO: Free the thread pool
    // 

    // Free the thread pool struct
    PARALLEL_REALLOC(p_thread_pool, 0);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            pointer_to_null_pointer:
                #ifndef NDEBUG
                    log_error("[parallel] [thread pool] Parameter \"pp_thread_pool\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}