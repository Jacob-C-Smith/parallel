/** !
 * Thread pool implementation
 * 
 * @file thread_pool.c
 *
 * @author Jacob Smith
 */

// parallel
#include <parallel/thread_pool.h>

// Function declarations
/** !
 * Wait for a job, then do the job
 * 
 * @param p_parameter the thread pool
 * 
 * @return 1 on success, 0 on error
 */
void *thread_pool_coordinator ( void *p_parameter )
{

    // Initialized data
    thread_pool *p_thread_pool = p_parameter;

    // Loop until close
    while (p_thread_pool->running)
    {

        mutex_lock(&p_thread_pool->_lock);

        // Wait until needed
        monitor_wait(&p_thread_pool->_monitor);

        printf("NOTIFY!!!\n");
        fflush(stdout);

        mutex_unlock(&p_thread_pool->_lock);
    }

    // Success
    return (void *) 1;
}

void *thread_pool_work ( void *p_parameter )
{

    // Initialized data
    thread_pool *p_thread_pool = p_parameter;



    mutex_lock(&p_thread_pool->_lock);

    // Wait until needed
    monitor_wait(&p_thread_pool->_monitor);

    printf("NOTIFY!!!\n");
    fflush(stdout);

    mutex_unlock(&p_thread_pool->_lock);

    // Success
    return (void *) 1;
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

int thread_pool_construct ( thread_pool **pp_thread_pool, int thread_quantity )
{
    
    // Argument check
    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    thread_pool *p_thread_pool = (void *) 0,
                  _thread_pool = { 0 };
    queue *jobs = (void *) 0;

    // Error check
    if ( thread_quantity > PARALLEL_THREAD_POOL_MAX_THREADS ) goto thread_pool_too_large;

    // Allocate memory for a thread pool
    if ( thread_pool_create(&p_thread_pool) == 0 ) goto failed_to_allocate_thread_pool;

    // Populate the thread pool struct
    _thread_pool = (thread_pool)
    {
        .thread_quantity = thread_quantity,
        .jobs            = jobs,
        .running         = true
    };

    // Construct a queue
    queue_construct(&_thread_pool.jobs);

    // Copy the thread pool struct to the heap
    memcpy(p_thread_pool, &_thread_pool, sizeof(thread_pool));

    // Create a thread
    parallel_thread_start(&_thread_pool.p_main, thread_pool_coordinator, p_thread_pool);

    // Create a mutex
    mutex_create(&p_thread_pool->_lock);

    // Create a monitor
    monitor_create(&p_thread_pool->_monitor);

    // Return a pointer to the caller
    *pp_thread_pool = p_thread_pool;

    // Success
    return 1;

    thread_pool_too_large:
        
        // Error
        return 0;

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
    }
}

int thread_pool_run ( thread_pool *p_thread_pool, void *job )
{
    
    // Argument check
    if ( p_thread_pool == (void *) 0 ) goto no_thread_pool;

    void *p_job = 0;
    int retries = 0;

    try:
    
    // Check for work
    if ( queue_empty(p_thread_pool->jobs) ) goto no_work;

    // Dequeue a job
    queue_dequeue(p_thread_pool, (const void **const)&p_job);

    // Start a thread to work on the job
    parallel_thread_start(
        &p_thread_pool->_p_threads[p_thread_pool->running_jobs],
        thread_pool_work,
        p_job
    );

    // This branch is run when there is no work to do
    no_work:

    // Success
    return 1;

    // Increment the retry counter
    retries++;

    // Try again?
    if ( retries < 5 ) goto try;

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
    p_thread_pool = PARALLEL_REALLOC(p_thread_pool, 0);

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
