/** !
 * @file parallel.c
 * 
 * @author Jacob Smith
 *
 * High level abstractions for parallelized computing
 */

// parallel
#include <parallel/parallel.h>

int parallel_init ( void ) 
{
    
    // Initialize log
    if ( log_init(0, true) == 0 ) goto failed_to_init_log;

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
