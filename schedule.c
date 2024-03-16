/** !
 * High level abstraction of a schedule
 * 
 * @file schedule.c
 *
 * @author Jacob Smith
 */

// parallel
#include <parallel/schedule.h>

// Preprocessor definitions
#define PARALLEL_SCHEDULE_NAME_LENGTH        255+1
#define PARALLEL_SCHEDULE_THREAD_NAME_LENGTH 255+1
#define PARALLEL_SCHEDULE_MAX_TASKS          64

// Forward declarations
struct parallel_schedule_thread_s;
struct parallel_schedule_task_s;

// Type definitions
typedef struct parallel_schedule_thread_s parallel_schedule_thread;
typedef struct parallel_schedule_task_s   parallel_schedule_task;

// Structure definitions
struct parallel_schedule_s
{
    char  _name[PARALLEL_SCHEDULE_NAME_LENGTH];
    dict *p_threads;
};

struct parallel_schedule_thread_s
{
    char   _name[PARALLEL_SCHEDULE_THREAD_NAME_LENGTH];
    size_t task_quantity;
    struct
    {

    } tasks[PARALLEL_SCHEDULE_MAX_TASKS];
};

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
int parallel_schedule_thread_create ( parallel_schedule_thread **pp_schedule_thread );

// TODO: Document
int parallel_schedule_thread_load ( parallel_schedule_thread **pp_thread, char *path );

// TODO: Document
int parallel_schedule_thread_load_as_json_text ( parallel_schedule_thread **pp_thread, char *text );

// TODO: Document
int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **pp_thread, json_value *p_value );

// TODO: Document
int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );

// Function definitions
int parallel_schedule_create ( parallel_schedule **pp_schedule )
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

int parallel_schedule_thread_create ( parallel_schedule_thread **pp_schedule_thread )
{

    // Argument check
    if ( pp_schedule_thread == (void *) 0 ) goto no_schedule_thread;

    // Initialized data
    parallel_schedule_thread *p_schedule_thread = (void *) 0;

    // Allocate memory for the schedule thread
    p_schedule_thread = PARALLEL_REALLOC(0, sizeof(parallel_schedule_thread));

    // Error check
    if ( p_schedule_thread == (void *) 0 ) goto no_mem;

    // Zero set memory
    memset(p_schedule_thread, 0, sizeof(parallel_schedule_thread));

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

int parallel_schedule_load ( parallel_schedule **pp_schedule, char *path );

int parallel_schedule_load_as_json_text ( parallel_schedule **pp_schedule, char *text );

int parallel_schedule_load_as_json_value ( parallel_schedule **pp_schedule, json_value *p_value )
{

    // Argument check
    if ( pp_schedule == (void *) 0 ) goto no_schedule;
    if ( p_value     == (void *) 0 ) goto no_value;

    // Initialized data
    //

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

            no_value: 
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_schedule_thread_load ( parallel_schedule_thread **pp_thread, char *path );

int parallel_schedule_thread_load_as_json_text ( parallel_schedule_thread **pp_thread, char *text );

int parallel_schedule_thread_load_as_json_value ( parallel_schedule_thread **pp_thread, json_value *p_value );

int parallel_schedule_start ( parallel_schedule *p_schedule )
{

    // TODO
    //

    // Success
    return 1;
}

int parallel_schedule_stop ( parallel_schedule *p_schedule )
{

    // TODO
    //

    // Success
    return 1;
}

int parallel_schedule_destroy ( parallel_schedule **pp_schedule )
{

    // TODO
    //

    // Success
    return 1;
}

int parallel_schedule_thread_destroy ( parallel_schedule_thread **pp_thread );
