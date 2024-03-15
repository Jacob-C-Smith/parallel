/** !
 * Example parallel program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>

// log
#include <log/log.h>

// parallel
#include <parallel/parallel.h>
#include <parallel/thread.h>
#include <parallel/thread_pool.h>
#include <parallel/schedule.h>

// Preprocessor defines
#define PARALLEL_THREADS_QUANTITY 4

// Forward declarations
/** !
 * Thread example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int parallel_thread_example ( int argc, const char *argv[] );

/** !
 * Thread pool example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int parallel_thread_pool_example ( int argc, const char *argv[] );

/** !
 * Schedule example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int parallel_schedule_example ( int argc, const char *argv[] );

/** !
 * Delay for [0 - 4] seconds, then print the parameter to standard out
 * 
 * @param p_parameter the parameter
 * 
 * @return void
 */
void print_something_to_standard_out ( void *p_parameter );

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialize parallel
    if ( parallel_init() == 0 ) goto failed_to_initialize_parallel;

    // Seed the random number generator
    srand(time(NULL));

    // TODO: Add tasks for scheduler
    //

    // Run the thread example program
    if ( parallel_thread_example(argc, argv) == 0 ) goto failed_to_run_thread_example;

    // Run the thread pool example program
    //if ( parallel_thread_pool_example(argc, argv) == 0 ) goto failed_to_run_thread_pool_example;

    // Run the schedule example program
    //if ( parallel_schedule_example(argc, argv) == 0 ) goto failed_to_run_thread_pool_example;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        
        failed_to_initialize_parallel:

            // Write an error message to standard out
            printf("Failed to initialize parallel!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_thread_example:

            // Print an error message
            printf("Failed to run thread example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_thread_pool_example:

            // Print an error message
            printf("Failed to run thread pool example!\n");

            // Error
            return EXIT_FAILURE;
    }
}

int parallel_thread_example ( int argc, const char *argv[] )
{
    
    // Initialized data
    parallel_thread *_p_parallel_threads[PARALLEL_THREADS_QUANTITY] = { 0 };

    // Start some threads
    for (size_t i = 0; i < PARALLEL_THREADS_QUANTITY; i++)

        // Start a thread
        if ( parallel_thread_start(&_p_parallel_threads[i], print_something_to_standard_out, i) == 0 ) goto failed_to_start_thread;

    // Wait for the threads to finish
    for (size_t i = 0; i < PARALLEL_THREADS_QUANTITY; i++)

        // Wait for the threads to finish
        if ( parallel_thread_join(&_p_parallel_threads[i]) == 0 ) goto failed_to_join_thread;
    
    // Success
    return 1;

    // Error handling
    {

        // Parallel errors
        {
            failed_to_start_thread:

                // Write an error message to standard out
                log_error("Failed to create parallel thread in call to function \"%s\"\n", __FUNCTION__);

                // Error
                return 0;

            failed_to_join_thread:
                
                // Write an error message to standard out
                log_error("Failed to join parallel thread in call to function \"%s\"\n", __FUNCTION__);

                // Error
                return 0;
        }
    }
}

int parallel_thread_pool_example ( int argc, const char *argv[] )
{

    // Initialized data
    thread_pool *p_thread_pool = (void *) 0;

    // Construct a thread pool
    if ( thread_pool_construct(&p_thread_pool, 8, print_something_to_standard_out) == 0 ) goto failed_to_construct_thread_pool;

    // Enqueue some tasks
    // for (size_t i = 1; i < 15; i++) thread_pool_enqueue(p_thread_pool, i);

    p_thread_pool->running = 1;

    sleep(6);

    // Success
    return 1;

    // Error handling
    {

        // parallel errors
        {
            failed_to_construct_thread_pool:
                
                // Write an error message to standard out
                log_error("Failed to construct thread pool!\n");

                // Error
                return 0;
        }
    }
}

int parallel_schedule_example ( int argc, const char *argv[] )
{

    // Success
    return 1;
}

void print_something_to_standard_out ( void *p_parameter )
{

    // Initialized data
    int delay = rand() % 5;

    // Sleep for 0-4 seconds
    sleep(delay);

    // Print the parameter to standard out
    printf("[parallel] [thread] %d finished in %d seconds\n", p_parameter, delay);

    // Flush standard out
    fflush(stdout);

    // Done
    return;
}
