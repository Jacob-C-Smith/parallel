/** !
 * High level abstraction of a thread pool
 * 
 * @file thread_pool.c
 *
 * @author Jacob Smith
 */

// Header
#include <parallel/thread_pool.h>

// Preprocessor definitions
#define PARALLEL_THREAD_POOL_NAME_LENGTH        (63 + 1)
#define PARALLEL_THREAD_POOL_THREAD_NAME_LENGTH (63 + 1)
#define PARALLEL_THREAD_POOL_TASK_NAME_LENGTH   (63 + 1)
#define PARALLEL_THREAD_POOL_MAX_THREADS        64
#define PARALLEL_THREAD_POOL_MAX_TASKS          256

// Forward declarations
struct parallel_thread_pool_thread_s;
struct parallel_thread_pool_task_s;
struct parallel_thread_pool_work_parameter_s;

// Type definitions
typedef struct parallel_thread_pool_thread_s         parallel_thread_pool_thread;
typedef struct parallel_thread_pool_task_s           parallel_thread_pool_task;
typedef struct parallel_thread_pool_work_parameter_s parallel_thread_pool_work_parameter;

// Structure definitions
struct parallel_thread_pool_task_s
{
    fn_parallel_task *pfn_task;
    monitor _monitor;
    char _name [PARALLEL_THREAD_POOL_TASK_NAME_LENGTH],
         _wait_thread [PARALLEL_THREAD_POOL_THREAD_NAME_LENGTH],
         _wait_task [PARALLEL_THREAD_POOL_TASK_NAME_LENGTH];
};

struct parallel_thread_pool_thread_s
{
    size_t task_quantity;
    bool running;
    parallel_thread *p_parallel_thread;
    char _name [PARALLEL_THREAD_POOL_THREAD_NAME_LENGTH];
    parallel_thread_pool_task tasks [];
};

struct parallel_thread_pool_work_parameter_s
{
    parallel_thread_pool        *p_thread_pool;
    parallel_thread_pool_thread *p_thread;
    void                        *p_parameter;
};

struct parallel_thread_pool_s
{
    monitor _montior;
    semaphore _semaphore;
    size_t running_threads;
    parallel_thread_pool_work_parameter _work_parameters[];
};

// Function declarations
/** !
 * Allocate memory for a scheudle thread
 * 
 * @param pp_thread_pool_thread return
 * 
 * @sa parallel_thread_pool_destroy
 * 
 * @return 1 on success, 0 on error
*/
int parallel_thread_pool_thread_create ( parallel_thread_pool_thread **pp_thread_pool_thread, size_t task_quantity );

/** !
 * Construct a named thread from a json value
 * 
 * @param pp_thread return
 * @param name      the name
 * @param value     the json value
 * 
 * @return 1 on success, 0 on error
 */
int parallel_thread_pool_thread_load_as_json_value ( parallel_thread_pool_thread **const pp_thread, const char *const name, const json_value *const p_value );

/** !
 * Worker thread loop
 * 
 * @param p_parameter who am I?
 * 
 * @return ret
 */
void *parallel_thread_pool_work ( parallel_thread_pool_work_parameter *p_parameter );

/** !
 * Main thread loop
 * 
 * @param p_parameter who am I?
 * 
 * @return ret
 */
void *parallel_thread_pool_main_work ( parallel_thread_pool_work_parameter *p_parameter );

// TODO: Document
int parallel_thread_pool_thread_destroy ( parallel_thread_pool_thread **pp_thread );

// Function definitions
int parallel_thread_pool_create ( parallel_thread_pool **const pp_thread_pool )
{

    // Argument check
    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    parallel_thread_pool *p_thread_pool = (void *) 0;

    // Allocate memory for the thread_pool
    p_thread_pool = PARALLEL_REALLOC(0, sizeof(parallel_thread_pool));

    // Error check
    if ( p_thread_pool == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_thread_pool, 0, sizeof(parallel_thread_pool));

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
                    log_error("[parallel] [thread_pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
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

int parallel_thread_pool_thread_create ( parallel_thread_pool_thread **const pp_thread_pool_thread, size_t task_quantity )
{

    // Argument check
    if ( pp_thread_pool_thread == (void *) 0 ) goto no_thread_pool_thread;

    // Initialized data
    parallel_thread_pool_thread *p_thread_pool_thread = (void *) 0;

    // Allocate memory for the thread_pool thread
    p_thread_pool_thread = PARALLEL_REALLOC(0, sizeof(parallel_thread_pool_thread) + (task_quantity * sizeof(parallel_thread_pool_task)));

    // Error check
    if ( p_thread_pool_thread == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_thread_pool_thread, 0, sizeof(parallel_thread_pool_thread));

    // Store the task quantity
    p_thread_pool_thread->task_quantity = task_quantity;

    // Return a pointer to the caller
    *pp_thread_pool_thread = p_thread_pool_thread;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_thread_pool_thread: 
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] [thread] Null pointer provided for parameter \"pp_thread_pool_thread\" in call to function \"%s\"\n", __FUNCTION__);
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

int parallel_thread_pool_load_as_json_value ( parallel_thread_pool **const pp_thread_pool, int thread_quantity )
{

    // Argument check
    if ( pp_thread_pool   ==        (void *) 0 ) goto no_thread_pool;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    dict *p_dict = p_value->object;
    const json_value *const p_name        = dict_get(p_dict, "name"),
                     *const p_threads     = dict_get(p_dict, "threads"),
                     *const p_main_thread = dict_get(p_dict, "main thread"),
                     *const p_repeat      = dict_get(p_dict, "repeat");
    parallel_thread_pool  _thread_pool  = { 0 }, 
                      *p_thread_pool = (void *) 0;

    // Check for missing properties
    if ( ! ( p_name && p_threads ) ) goto missing_properties;

    // Parse the name property
    if ( p_name->type == JSON_VALUE_STRING )
    {

        // Initialized data
        char   *p_name_string = p_name->string;
        size_t  len           = strlen(p_name_string);

        // Error check
        if ( len < 1 ) goto name_property_too_short;
        if ( len > PARALLEL_THREAD_POOL_NAME_LENGTH ) goto name_property_too_long;
        
        // Copy the name into the thread_pool struct
        strncpy(_thread_pool._name, p_name_string, len);

        // Null terminate the copied string
        _thread_pool._name[len] = '\0';
    } 
    
    // Error check
    if ( p_main_thread == (void *) 0 ) goto no_main_thread; 

    // Parse the main thread property
    if ( p_main_thread->type == JSON_VALUE_STRING )
    {

        // Initialized data
        char   *p_main_thread_string = p_main_thread->string;
        size_t  len                  = strlen(p_main_thread_string);
        
        // Error check
        if ( len < 1 ) goto main_thread_property_too_short;
        if ( len > PARALLEL_THREAD_POOL_THREAD_NAME_LENGTH ) goto main_thread_property_too_long;
        
        // Copy the name into the thread_pool struct
        strncpy(_thread_pool._main_thread_name, p_main_thread_string, len);

        // Null terminate the copied string
        _thread_pool._main_thread_name[len] = '\0';
    } 

    // Default
    else goto wrong_main_thread_type;
    
    no_main_thread:
    // Parse the threads property
    if ( p_threads->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict       *p_dict           = p_threads->object;
        size_t      threads_quantity = dict_keys(p_dict, (void *) 0 );
        const char *_keys[PARALLEL_THREAD_POOL_MAX_THREADS]     = { 0 };
        json_value *_p_values[PARALLEL_THREAD_POOL_MAX_THREADS] = { 0 };
        
        // Error check
        if ( threads_quantity == 0 ) goto threads_property_is_empty;
        if ( threads_quantity > PARALLEL_THREAD_POOL_MAX_THREADS ) goto threads_property_is_too_large;

        // Store the names of the threads
        dict_keys(p_dict, _keys);

        // Store the values of the threads
        dict_values(p_dict, _p_values);

        // Construct a dictionary for the threads
        (void) dict_construct(&_thread_pool.p_threads, threads_quantity * 2, 0);

        // Iterate over each thread
        for (size_t i = 0; i < threads_quantity; i++)
        {

            // Initialized data
            parallel_thread_pool_thread *p_thread = (void *) 0;

            // Construct a thread_pool thread
            if ( parallel_thread_pool_thread_load_as_json_value(&p_thread, _keys[i], _p_values[i]) == 0 ) goto failed_to_create_thread;

            // Add the thread_pool thread to the thread_pool
            dict_add(_thread_pool.p_threads, p_thread->_name, p_thread);
        }
    }

    // Default
    else goto wrong_threads_type;

    // Jump ahead
    if ( p_repeat == (void *) 0 ) goto no_repeat_property;

    // Parse the repeat property
    if ( p_repeat->type == JSON_VALUE_BOOLEAN ) 

        // Store the repeat property
        _thread_pool.repeat = p_repeat->boolean;

    // Default
    else goto wrong_repeat_type;

    no_repeat_property:

    // Validate the thread_pool
    {

        // Initialized data
        size_t thread_quantity = dict_values(_thread_pool.p_threads, 0);
        parallel_thread_pool_thread *_p_threads [PARALLEL_THREAD_POOL_MAX_THREADS] = { 0 };
        parallel_thread_pool_thread *p_thread = (void *) 0;
        parallel_thread_pool_task   *p_task = (void *) 0;
        
        // Store the threads from the thread_pool
        dict_values(_thread_pool.p_threads, _p_threads);

        // Iterate over each thread
        for (size_t i = 0; i < thread_quantity; i++)
        {
            
            // Store the thread
            p_thread = _p_threads[i];

            // Iterate through each task
            for (size_t j = 0; j < p_thread->task_quantity; j++)
            {
                
                // Store the task
                p_task = &p_thread->tasks[j];

                // Will this task wait for something else?
                if ( p_task->dependent )
                {
                    
                    // Initialized data
                    parallel_thread_pool_thread *p_dependency_thread = dict_get(_thread_pool.p_threads, p_task->_wait_thread);

                    // TODO: Error check
                    if ( p_dependency_thread == (void *) 0 );

                    // Iterate through each task
                    for (size_t k = 0; k < p_dependency_thread->task_quantity; k++)
                    {
                        
                        // Initialized data
                        parallel_thread_pool_task *i_dependency_task = &p_dependency_thread->tasks[k];
                        struct { char *th, *ta; } thread_task[64] = { 0 };

                        // Will the task wait on this task?
                        i_dependency_task->dependency = ( strcmp(i_dependency_task->_name, p_task->_wait_task) == 0 );

                        if ( i_dependency_task->dependency )
                        {
                            thread_task[i].ta = p_thread->_name,
                            thread_task[i].th = p_task->_name;
                            i_dependency_task->dependency = true;
                            i_dependency_task->dependencies++;
                        }
                    }
                }
            }
        }
    
        // Iterate over each thread
        for (size_t i = 0; i < thread_quantity; i++)
        {
            
            // Store the thread
            p_thread = _p_threads[i];

            // Iterate through each task
            for (size_t j = 0; j < p_thread->task_quantity; j++)
                
                // Store the task
                p_task = &p_thread->tasks[j];            
        }
    }

    // Allocate memory for a thread_pool
    if ( parallel_thread_pool_create(&p_thread_pool) == 0 ) goto failed_to_create_thread_pool;

    // Copy the thread_pool from the stack to the heap
    memcpy(p_thread_pool, &_thread_pool, sizeof(parallel_thread_pool));

    // Construct a monitor for the thread_pool
    monitor_create(&p_thread_pool->_montior);

    // Construct a mutex for the thread_pool
    mutex_create(&p_thread_pool->_lock);

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
                    log_error("[parallel] [thread_pool] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value: 
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            missing_properties:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Not enough properties to construct thread_pool in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"name\" property of thread_pool object must be of type [ string ] in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_threads_type:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"threads\" property of thread_pool object must be of type [ object ] in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_main_thread_type:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"main thread\" property of thread_pool object must be of type [ object ] in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_repeat_type:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"repeat\" property of thread_pool object must be of type [ boolean ] in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_property_too_long:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"name\" property of thread_pool object must be less than %d characters in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", PARALLEL_THREAD_POOL_NAME_LENGTH, __FUNCTION__);
                #endif

                // Error
                return 0;

            name_property_too_short:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"name\" property of thread_pool object must be at least 1 character long in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            main_thread_property_too_short:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"main thread\" property of thread_pool object must be at least 1 character long in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            main_thread_property_too_long:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"main thread\" property of thread_pool object must be less than %d characters in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", PARALLEL_THREAD_POOL_THREAD_NAME_LENGTH, __FUNCTION__);
                #endif

                // Error
                return 0;

            threads_property_is_empty:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"threads\" property of thread_pool object must contain at least 1 property in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            threads_property_is_too_large:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] \"threads\" property of thread_pool object must contain %d or fewer properties in call to function \"%s\"\n\"Refer to thread_pool schema: [TODO: Schedule schema URL] \n", PARALLEL_THREAD_POOL_MAX_THREADS, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    
        // Parallel errors
        {

            failed_to_create_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Failed to allocate thread_pool in call to functon \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Failed to create thread_poolr thread in call to functon \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_thread_pool_start ( parallel_thread_pool *const p_thread_pool )
{

    // Argument check
    if ( p_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    //

    // Start the main thread
    parallel_thread_pool_main_work(p_thread_pool);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Null pointer provided for parameter \"p_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Parallel errors
        {
            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Failed to spawn thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_thread_pool_wait_idle ( parallel_thread_pool *const p_thread_pool )
{
    
    // Spin until all threads are done
    while (p_thread_pool->running_threads) sleep(0);

    // Success
    return 1;
}

int parallel_thread_pool_pause ( parallel_thread_pool *const p_thread_pool )
{

    // TODO:
    
    // Success
    return 1;
} 

int parallel_thread_pool_stop ( parallel_thread_pool *const p_thread_pool )
{

    // Argument check
    if ( p_thread_pool == (void *) 0 ) goto no_thread_pool;

    // Initialized data
    size_t thread_quantity = dict_values(p_thread_pool->p_threads, 0);
    parallel_thread_pool_thread *_p_threads[PARALLEL_THREAD_POOL_MAX_THREADS] = { 0 };

    // Clear the repeat flag
    p_thread_pool->repeat = false;

    // Store the threads from the thread_pool
    dict_values(p_thread_pool->p_threads, _p_threads);

    // Iterate over each thread
    for (size_t i = 0; i < thread_quantity; i++)
    {
        
        // Initialized data
        parallel_thread_pool_thread *p_thread = _p_threads[i];

        // Skip the main thread
        if ( strcmp(p_thread_pool->_main_thread_name, p_thread->_name) == 0 ) continue;

        // Clear the running flag
        p_thread->running = false;

        // Cancel the thread
        parallel_thread_cancel(p_thread->p_parallel_thread);

        // Join the thread
        if ( parallel_thread_join(&p_thread->p_parallel_thread) == 0 ) goto failed_to_destroy_thread;
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_thread_pool:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Null pointer provided for parameter \"p_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Parallel errors
        {
            failed_to_destroy_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Failed to join thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_thread_pool_destroy ( parallel_thread_pool **const pp_thread_pool )
{

    // TODO
    //

    // Success
    return 1;
}

void *parallel_thread_pool_work ( parallel_thread_pool_work_parameter *p_parameter )
{

    // Argument check
    if ( p_parameter == (void *) 0 ) goto no_work_parameter;

    // Initialized data
    parallel_thread_pool        *p_thread_pool        = p_parameter->p_thread_pool;
    parallel_thread_pool_thread *p_thread_pool_thread = p_parameter->p_thread;
    parallel_thread_pool_task   *i_task            = (void *) 0;
    
    // Lock
    mutex_lock(&p_thread_pool->_lock);

    // Set the running flag
    p_thread_pool_thread->running = true;

    // Increment the quantity of running threads
    p_thread_pool->running_threads++;

    // Unlock
    mutex_unlock(&p_thread_pool->_lock);

    // Wait for the signal
    monitor_wait(&p_thread_pool->_montior);

    turnover:

    // Iterate through each task
    for (size_t i = 0; i < p_thread_pool_thread->task_quantity; i++)
    {

        // Initialized data
        i_task = &p_thread_pool_thread->tasks[i];

        // Wait
        if ( i_task->dependent ) goto wait_logic;

        // Done waiting
        done:

        // Run the task
        i_task->pfn_task(p_parameter->p_thread_pool->p_parameter);
        
        // Signal
        if ( i_task->dependency ) monitor_notify_all(&i_task->_monitor);
    }
    
    // Repeat?
    if ( p_thread_pool->repeat ) goto turnover;

    // Lock
    mutex_lock(&p_thread_pool->_lock);

    // Clear the running flag
    p_thread_pool_thread->running = false;

    // Decrement the quantity of running threads
    p_thread_pool->running_threads--;

    // Unlock
    mutex_unlock(&p_thread_pool->_lock);

    // Success
    return (void *) 1;

    wait_logic:
    {

        // Initialized data
        char *thread_name = i_task->_wait_thread;
        const parallel_thread_pool_thread *const p_thread_pool_thread = dict_get(p_thread_pool->p_threads, thread_name);
        
        // Find the monitor 
        for (size_t i = 0; i < p_thread_pool_thread->task_quantity; i++)
        {
        
            // Check the monitor
            if ( strcmp(p_thread_pool_thread->tasks[i]._name, i_task->_wait_task) == 0 ) 
            {

                // Wait
                monitor_wait(&p_thread_pool_thread->tasks[i]._monitor);

                // Done
                goto done;
            }
        }

        // Default
        goto failed_to_find_monitor;
    }

    // Error handling
    {

        // Argument errors
        {
            no_work_parameter:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Null pointer provided for parameter \"p_parameter\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Parallel errors
        {
            failed_to_find_monitor:
                #ifndef NDEBUG
                    log_error("[parallel] [thread_pool] Failed to find monitor for dependant task on thread\"%s\" in call to function \"%s\"\n", p_thread_pool_thread->_name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_thread_pool_thread_destroy ( parallel_thread_pool_thread **pp_thread );
