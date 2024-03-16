/** !
 * High level abstractions for parallelized computing
 * 
 * @file parallel.c
 *
 * @author Jacob Smith
 */

// parallel
#include <parallel/parallel.h>

// Static data
static dict *parallel_parallel_tasks = 0;

// Forward declarations
void parallel_quit ( void );

int parallel_init ( void ) 
{

    // Initialize log
    if ( log_init(0, true) == 0 ) goto failed_to_init_log;

    // Construct a dictionary for parallel tasks
    dict_construct(&parallel_parallel_tasks, 256, 0);

    // Success
    return 1;

    // Error handling
    {

        // log errors
        {
            failed_to_init_log:
                #ifndef NDEBUG
                    printf("[parallel] Failed to initialize log\n");
                #endif

                // Error
                return 0;
        }
    }
}

int parallel_register_task ( const char *const name, fn_parallel_task *pfn_parallel_task )
{

    // TODO: Argument check

    // Store the task
    dict_add(parallel_parallel_tasks, name, pfn_parallel_task);

    // Success
    return 1;
}

int parallel_find_task ( const char *const name, fn_parallel_task **p_pfn_parallel_task )
{

    // TODO: Argument check
    fn_parallel_task *r = dict_get(parallel_parallel_tasks, name);

    *p_pfn_parallel_task = r;

    // Success
    return 1;
}

void parallel_quit ( void )
{

    // Clean up
    dict_destroy(&parallel_parallel_tasks);
}