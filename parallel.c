/** !
 * High level abstractions for parallelized computing
 * 
 * @file parallel.c
 *
 * @author Jacob Smith
 */

// parallel
#include <parallel/parallel.h>

int parallel_init ( void ) 
{

    // External data
    extern dict *parallel_schedule_tasks;

    // Initialize log
    if ( log_init(0, true) == 0 ) goto failed_to_init_log;

    // Construct a dictionary for parallel tasks
    dict_construct(&parallel_schedule_tasks, 256, 0);

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
