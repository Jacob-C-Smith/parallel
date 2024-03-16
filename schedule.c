/** !
 * High level abstraction of a schedule
 * 
 * @file schedule.c
 *
 * @author Jacob Smith
 */

// Header
#include <parallel/schedule.h>

// Preprocessor definitions
#define PARALLEL_SCHEDULE_NAME_LENGTH        63+1
#define PARALLEL_SCHEDULE_THREAD_NAME_LENGTH 63+1
#define PARALLEL_SCHEDULE_TASK_NAME_LENGTH   63+1
#define PARALLEL_SCHEDULE_MAX_THREADS        64
#define PARALLEL_SCHEDULE_MAX_TASKS          256

// Forward declarations
struct parallel_schedule_thread_s;
struct parallel_schedule_task_s;

// Type definitions
typedef struct parallel_schedule_thread_s parallel_schedule_thread;
typedef struct parallel_schedule_task_s   parallel_schedule_task;

// Structure definitions
struct parallel_schedule_task_s
{
    bool dependent;
    fn_scheduler_task *pfn_task;
    char _name [PARALLEL_SCHEDULE_TASK_NAME_LENGTH],
         _wait_thread [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH],
         _wait_task [PARALLEL_SCHEDULE_TASK_NAME_LENGTH];
};

struct parallel_schedule_s
{
    dict *p_threads;
    char  _name [PARALLEL_SCHEDULE_NAME_LENGTH],
          _main_thread [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH];
};

struct parallel_schedule_thread_s
{
    size_t task_quantity;
    bool running;
    parallel_thread *p_parallel_thread;
    char _name [PARALLEL_SCHEDULE_THREAD_NAME_LENGTH];
    parallel_schedule_task tasks [];
};

// Static data
dict *parallel_schedule_tasks = 0;

// Function declarations
/** !
 * Allocate memory for a scheudle thread
 * 
 * @param pp_schedule_thread return
 * 
 * @sa parallel_schedule_destroy
 * 
 * @return 1 on success, 0 on error
*/
int parallel_schedule_thread_create ( parallel_schedule_thread **pp_schedule_thread, size_t task_quantity );

/** !
 * Construct a named thread from a json value
 * 
 * @param pp_thread return
 * @param name      the name
 * @param value     the json value
 * 
 * @return 1 on success, 0 on error
 */
int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **const pp_thread, const char *const name, const json_value *const p_value );

/** !
 * Worker thread loop
 * 
 * @param p_thread who am I?
 * 
 * @return void
 */
void parallel_schedule_work ( parallel_schedule_thread *p_thread );

// TODO: Document
int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );

// Function definitions
int parallel_schedule_create ( parallel_schedule **const pp_schedule )
{

    // Argument check
    if ( pp_schedule == (void *) 0 ) goto no_schedule;

    // Initialized data
    parallel_schedule *p_schedule = (void *) 0;

    // Allocate memory for the schedule
    p_schedule = PARALLEL_REALLOC(0, sizeof(parallel_schedule));

    // Error check
    if ( p_schedule == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_schedule, 0, sizeof(parallel_schedule));

    // Return a pointer to the caller
    *pp_schedule = p_schedule;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
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

int parallel_schedule_thread_create ( parallel_schedule_thread **const pp_schedule_thread, size_t task_quantity )
{

    // Argument check
    if ( pp_schedule_thread == (void *) 0 ) goto no_schedule_thread;

    // Initialized data
    parallel_schedule_thread *p_schedule_thread = (void *) 0;

    // Allocate memory for the schedule thread
    p_schedule_thread = PARALLEL_REALLOC(0, sizeof(parallel_schedule_thread) + (task_quantity * sizeof(parallel_schedule_task)));

    // Error check
    if ( p_schedule_thread == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_schedule_thread, 0, sizeof(parallel_schedule_thread));

    // Store the task quantity
    p_schedule_thread->task_quantity = task_quantity;

    // Return a pointer to the caller
    *pp_schedule_thread = p_schedule_thread;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule_thread: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] [thread] Null pointer provided for parameter \"pp_schedule_thread\" in call to function \"%s\"\n", __FUNCTION__);
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

int parallel_schedule_load ( parallel_schedule **pp_schedule, const char *const path )
{

    // Argument check
    if ( pp_schedule == (void *) 0 ) goto no_schedule;
    if ( path        == (void *) 0 ) goto no_path;

    // Initialized data
    size_t      file_size       = 0;
    char       *p_file_contents = (void *) 0;
    json_value *p_value         = (void *) 0;
    
    // Query the size of the file
    file_size = load_file(path, 0, true);

    // Error check
    if ( file_size == 0 ) goto invalid_file;
    
    // Allocate memory for the file
    p_file_contents = PARALLEL_REALLOC(0, file_size);

    // Error check
    if ( p_file_contents == (void *) 0 ) goto no_mem;

    // Load the file
    load_file(path, p_file_contents, 0);

    // Parse the file into a json value
    if ( parse_json_value(p_file_contents, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

    // Construct a schedule
    if ( parallel_schedule_load_as_json_value(pp_schedule, p_value) == 0 ) goto failed_to_construct_schedule;

    // Clean up
    PARALLEL_REALLOC(p_file_contents, 0);
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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
        
        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Can not load test file in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
        
        // JSON errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // Parallel errors
        {
            failed_to_construct_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to construct schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int parallel_schedule_load_as_json_value ( parallel_schedule **const pp_schedule, const json_value *const p_value )
{

    // Argument check
    if ( pp_schedule   ==        (void *) 0 ) goto no_schedule;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    parallel_schedule  _schedule  = { 0 }, 
                      *p_schedule = (void *) 0;
    dict *p_dict = p_value->object;
    const json_value *const p_name        = dict_get(p_dict, "name"),
                     *const p_main_thread = dict_get(p_dict, "main thread"),
                     *const p_threads     = dict_get(p_dict, "threads");

    // Check for missing properties
    if ( ! ( p_name && p_threads && p_main_thread ) ) goto missing_properties;

    // Parse the name property
    if ( p_name->type == JSON_VALUE_STRING )
    {

        // Initialized data
        char   *p_name_string = p_name->string;
        size_t  len           = strlen(p_name_string);

        // Error check
        if ( len < 1 ) goto name_property_too_short;
        if ( len > PARALLEL_SCHEDULE_NAME_LENGTH ) goto name_property_too_long;
        
        // Copy the name into the schedule struct
        strncpy(_schedule._name, p_name_string, len);

        // Null terminate the copied string
        _schedule._name[len] = '\0';
    } 

    // Default
    else goto wrong_name_type;

    // Parse the main thread property
    if ( p_name->type == JSON_VALUE_STRING )
    {

        // Initialized data
        char   *p_main_thread_string = p_main_thread->string;
        size_t  len                  = strlen(p_main_thread_string);

        // Error check
        if ( len < 1 ) goto main_thread_property_too_short;
        if ( len > PARALLEL_SCHEDULE_THREAD_NAME_LENGTH - 1 ) goto main_thread_property_too_long;
        
        // Copy the name into the schedule struct
        strncpy(_schedule._main_thread, p_main_thread_string, len);

        // Null terminate the copied string
        _schedule._main_thread[len] = '\0';
    } 

    // Default
    else goto wrong_name_type;

    // Parse the threads property
    if ( p_threads->type == JSON_VALUE_OBJECT )
    {

        // Initialized data
        dict       *p_dict           = p_threads->object;
        size_t      threads_quantity = dict_keys(p_dict, (void *) 0 );
        const char *_keys[PARALLEL_SCHEDULE_MAX_THREADS]     = { 0 };
        json_value *_p_values[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };
        
        // Error check
        if ( threads_quantity == 0 ) goto threads_property_is_empty;
        if ( threads_quantity > PARALLEL_SCHEDULE_MAX_THREADS ) goto threads_property_is_too_large;

        // Store the names of the threads
        dict_keys(p_dict, _keys);

        // Store the values of the threads
        dict_values(p_dict, _p_values);

        // Construct a dictionary for the threads
        (void) dict_construct(&_schedule.p_threads, threads_quantity * 2, 0);

        // Iterate over each thread
        for (size_t i = 0; i < threads_quantity; i++)
        {

            // Initialized data
            parallel_schedule_thread *p_thread = (void *) 0;

            // Construct a schedule thread
            if ( parallel_schedule_thread_load_as_json_value(&p_thread, _keys[i], _p_values[i]) == 0 ) goto failed_to_create_thread;

            // Add the schedule thread to the schedule
            dict_add(_schedule.p_threads, p_thread->_name, p_thread);
        }
    }

    // Default
    else goto wrong_threads_type;

    // Allocate memory for a schedule
    if ( parallel_schedule_create(&p_schedule) == 0 ) goto failed_to_create_schedule;

    // Copy the schedule from the stack to the heap
    memcpy(p_schedule, &_schedule, sizeof(parallel_schedule));

    // Return a pointer to the caller
    *pp_schedule = p_schedule;

    // Success
    return 1;

    failed_to_create_schedule:

        // Error
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_schedule: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            missing_properties:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Not enough properties to construct schedule in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"name\" property of schedule object must be of type [ string ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_threads_type:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"threads\" property of schedule object must be of type [ object ] in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_property_too_long:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"name\" property of schedule object must be less than %d characters in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", PARALLEL_SCHEDULE_NAME_LENGTH, __FUNCTION__);
                #endif

                // Error
                return 0;

            name_property_too_short:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"name\" property of schedule object must be at least 1 character long in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            main_thread_property_too_long:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"main thread\" property of schedule object must be less than %d characters in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", PARALLEL_SCHEDULE_THREAD_NAME_LENGTH, __FUNCTION__);
                #endif

                // Error
                return 0;

            main_thread_property_too_short:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"main thread\" property of schedule object must be at least 1 character long in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            threads_property_is_empty:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"threads\" property of schedule object must contain at least 1 property in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            threads_property_is_too_large:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] \"threads\" property of schedule object must contain %d or fewer properties in call to function \"%s\"\n\"Refer to schedule schema: [TODO: Schedule schema URL] \n", PARALLEL_SCHEDULE_MAX_THREADS, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    
        // Parallel errors
        {

            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to create scheduler thread in call to functon \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **const pp_schedule_thread, const char *const name, const json_value *const p_value )
{

    // Argument check
    if ( pp_schedule_thread ==       (void *) 0 ) goto no_thread;
    if ( name               ==       (void *) 0 ) goto no_name;
    if ( p_value            ==       (void *) 0 ) goto no_value;
    if ( p_value->type      != JSON_VALUE_ARRAY ) goto wrong_type;

    // Initialized data
    parallel_schedule_thread *p_schedule_thread = (void *) 0;
    array *p_array = p_value->list;
    size_t task_quantity = array_size(p_array),
           thread_name_len = strlen(name);
    char *error_state = (void *) 0;

    // Allocate memory for a schedule thread
    if ( parallel_schedule_thread_create(&p_schedule_thread, task_quantity) == 0 ) goto failed_to_allocate_schedule_thread;

    // Copy the name of the thread
    strncpy(p_schedule_thread->_name, name, thread_name_len);

    // Iterate through the array
    for (size_t i = 0; i < task_quantity; i++)
    {
        
        // Initialized data
        const json_value *const p_ith_value = (void *) 0;
        
        // Store the ith json value
        (void) array_index(p_array, i, &p_ith_value);

        // Error check
        if ( p_ith_value->type == JSON_VALUE_OBJECT ) 
        {

            // Initialized data
            dict *p_dict = p_ith_value->object;
            const json_value *const p_task = dict_get(p_dict, "task"),
                             *const p_wait = dict_get(p_dict, "wait");
            
            // Check for missing properties
            if ( p_task == (void *) 0 ) goto missing_properties;

            // Parse the task property
            if ( p_task->type == JSON_VALUE_STRING )
            {
                
                // Initialized data
                size_t len = 0;
                fn_scheduler_task *pfn_task = dict_get(parallel_schedule_tasks, p_task->string);

                // Error check
                if ( pfn_task == 0 )
                {

                    // Update the error state
                    error_state = p_task->string;

                    // Handle the error
                    goto unrecognized_task;
                }

                // Store the length of the task name
                len = strlen(p_task->string);

                // TODO: Bounds check
                //

                // Copy the task name
                strncpy(p_schedule_thread->tasks[i]._name, p_task->string, len);

                // Store the task function pointer
                p_schedule_thread->tasks[i].pfn_task = pfn_task;
            }

            // Default
            else goto wrong_task_task_type;

            // Set the dependent flag
            p_schedule_thread->tasks[i].dependent = (bool) p_wait;

            // Parse the wait property
            if ( p_wait )
            if ( p_wait->type == JSON_VALUE_STRING )
            {

                // Initialized data
                char *wait_thread      = p_wait->string, 
                     *wait_task        = strchr(wait_thread, ':');
                size_t wait_thread_len = 0,
                       wait_task_len   = 0;

                // Error check
                if ( wait_task == (void *) 0 ) goto no_colon_delimiter;

                // Compute the length of the wait thread
                wait_thread_len = (wait_task - wait_thread);

                // Increment past the ':'
                wait_task++;

                // Store the length of the wait task
                wait_task_len = strlen(wait_task);

                // Error check
                if ( wait_task_len   > PARALLEL_SCHEDULE_TASK_NAME_LENGTH - 1 ) goto wait_task_too_long;
                if ( wait_task_len   < 1 ) goto wait_task_too_short;
                if ( wait_thread_len > PARALLEL_SCHEDULE_THREAD_NAME_LENGTH - 1 ) goto wait_thread_too_long;
                if ( wait_thread_len < 1 ) goto wait_thread_too_short;

                // Copy the wait thread
                strncpy(p_schedule_thread->tasks[i]._wait_thread, wait_thread, wait_thread_len);

                // Copy the wait task
                strncpy(p_schedule_thread->tasks[i]._wait_task, wait_task, wait_task_len);
            }

            // Default 
            else goto wrong_task_wait_type;
        }

        // Default
        else goto wrong_task_type;
    }

    /*
    for (size_t i = 0; i < task_quantity; i++)
    {
    
        if ( p_schedule_thread->tasks[i].dependent )
        
            printf(
                "%s waits on %s on %s\n",
                p_schedule_thread->tasks[i]._name,
                p_schedule_thread->tasks[i]._wait_task,
                p_schedule_thread->tasks[i]._wait_thread
            );

        else

            printf(
                "%s waits on nothing\n",
                p_schedule_thread->tasks[i]._name
            );
    }*/
        
    // Return a pointer to the caller
    *pp_schedule_thread = p_schedule_thread;

    // Success
    return 1;

    // TODO

    // Argument errors
    no_thread:
    no_name:
    no_value:
    wrong_type:

    // JSON errors
    missing_properties:
    wrong_task_type:
    wrong_task_task_type:
    wrong_task_wait_type:
    no_colon_delimiter:
    wait_task_too_long:
    wait_task_too_short:
    wait_thread_too_long:
    wait_thread_too_short:

    // Parallel errors
    failed_to_allocate_schedule_thread:

        // Error
        return 0;


    // Error handling
    {

        // Argument errors
        {

        }

        // Parallel errors
        {
            unrecognized_task:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Unrecognized task \"%s\" was encountered while constructing therad \"%s\" in call to function \"%s\"\n", error_state, name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }   
}

int parallel_schedule_start ( parallel_schedule *const p_schedule )
{

    // Argument check
    if ( p_schedule == (void *) 0 ) goto no_schedule;

    // Initialized data
    size_t thread_quantity = dict_values(p_schedule->p_threads, 0);
    parallel_schedule_thread *_p_threads[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };

    // Store the threads from the schedule
    dict_values(p_schedule->p_threads, _p_threads);

    // Iterate over each thread
    for (size_t i = 0; i < thread_quantity; i++)
    {
        
        // Initialized data
        parallel_schedule_thread *p_thread = _p_threads[i];

        // Set the running flag
        p_thread->running = true;

        // Spawn the thread
        if ( parallel_thread_start(&p_thread->p_parallel_thread, parallel_schedule_work, p_thread) == 0 ) goto failed_to_create_thread;
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Parallel errors
        {
            failed_to_create_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to spawn thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_schedule_stop ( parallel_schedule *const p_schedule )
{

    // Argument check
    if ( p_schedule == (void *) 0 ) goto no_schedule;

    // Initialized data
    size_t thread_quantity = dict_values(p_schedule->p_threads, 0);
    parallel_schedule_thread *_p_threads[PARALLEL_SCHEDULE_MAX_THREADS] = { 0 };

    // Store the threads from the schedule
    dict_values(p_schedule->p_threads, _p_threads);

    // Iterate over each thread
    for (size_t i = 0; i < thread_quantity; i++)
    {
        
        // Initialized data
        parallel_schedule_thread *p_thread = _p_threads[i];

        // Clear the running flag
        p_thread->running = false;

        // Join the thread
        if ( parallel_thread_join(&p_thread->p_parallel_thread) == 0 ) goto failed_to_destroy_thread;
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Parallel errors
        {
            failed_to_destroy_thread:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to join thread in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_schedule_register_task ( const char *const name, fn_scheduler_task *pfn_scheduler_task )
{

    // TODO: Argument check
    //

    // Store the task
    dict_add(parallel_schedule_tasks, name, pfn_scheduler_task);

    // Success
    return 1;
}

int parallel_schedule_destroy ( parallel_schedule **const pp_schedule )
{

    // TODO
    //

    // Success
    return 1;
}

void parallel_schedule_work ( parallel_schedule_thread *p_thread )
{

    for (size_t i = 0; i < p_thread->task_quantity; i++)
    
        
        p_thread->tasks[i].pfn_task(p_thread->_name);
    
    
    return;
}

int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );
