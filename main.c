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
#define PARALLEL_THREADS_QUANTITY  4
#define PARALLEL_THREADS_MAX_DELAY 4

// Enumeration definitions
enum parallel_examples_e
{
    PARALLEL_THREAD_EXAMPLE      = 0,
    PARALLEL_THREAD_POOL_EXAMPLE = 1,
    PARALLEL_SCHEDULE_EXAMPLE    = 2,
    PARALLEL_EXAMPLES_QUANTITY   = 3
};

// Forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

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
void *print_something_to_standard_out ( void *p_parameter );

/** !
 * Alice tells a joke
 * 
 * @param null_pointer 0
 * 
 * @return void
 */
void *alice_joke ( void *null_pointer );

/** !
 * Bob tells a joke
 * 
 * @param null_pointer 0
 * 
 * @return void
 */
void *bob_joke ( void *null_pointer );

/** !
 * Charlie tells a joke
 * 
 * @param null_pointer 0
 * 
 * @return void
 */
void *charlie_joke ( void *null_pointer );

/** !
 * Someone starts laughing
 * 
 * @param null_pointer 0
 * 
 * @return void
*/
void *laugh ( void *null_pointer );

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    bool examples_to_run[PARALLEL_EXAMPLES_QUANTITY] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, examples_to_run);

    // Formatting
    log_info(
        "╭──────────────────╮\n"\
        "│ parallel example │\n"\
        "╰──────────────────╯\n"\
        "The parallel library provides high level abstractions for parallel computing.\n"\
        "Parallel provides %d abstractions. The thread, the thread pool, and the schedule.\n\n"\
        "A thread is the most primitive abstraction in parallel computing.\n"\
        "A thread pool is an abstraction for a collection of threads.\n"\
        "A schedule is an abstraction for coordinating logical units of work over time. \n\n",
        PARALLEL_EXAMPLES_QUANTITY
    );

    //////////////////////
    // Run the examples //
    //////////////////////

    // Run the thread example program
    if ( examples_to_run[PARALLEL_THREAD_EXAMPLE] )

        // Error check
        if ( parallel_thread_example(argc, argv) == 0 ) goto failed_to_run_thread_example;

    // Run the thread pool example program
    if ( examples_to_run[PARALLEL_THREAD_POOL_EXAMPLE] )

        // Error check
        if ( parallel_thread_pool_example(argc, argv) == 0 ) goto failed_to_run_thread_pool_example;

    // Run the schedule example program
    if ( examples_to_run[PARALLEL_SCHEDULE_EXAMPLE] )

        // Error check
        if ( parallel_schedule_example(argc, argv) == 0 ) goto failed_to_run_schedule_example;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        failed_to_run_thread_example:

            // Print an error message
            log_error("Error: Failed to run thread example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_thread_pool_example:

            // Print an error message
            log_error("Error: Failed to run thread pool example!\n");

            // Error
            return EXIT_FAILURE;
        
        failed_to_run_schedule_example:
            
            // Print an error message
            log_error("Error: Failed to run schedule example!\n");

            // Error
            return EXIT_FAILURE;   
    }
}

void print_usage ( const char *argv0 )
{

    // Argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [thread] [thread-pool] [schedule]\n", argv0);

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc == 1 ) goto all_examples;

    // Error check
    if ( argc > PARALLEL_EXAMPLES_QUANTITY + 1 ) goto invalid_arguments;

    // Iterate through each command line argument
    for (size_t i = 1; i < argc; i++)
    {
        
        // Thread example?
        if ( strcmp(argv[i], "thread") == 0 )

            // Set the thread example flag
            examples_to_run[PARALLEL_THREAD_EXAMPLE] = true;

        // Thread pool example?
        else if ( strcmp(argv[i], "thread-pool") == 0 )
            
            // Set the thread pool example flag
            examples_to_run[PARALLEL_THREAD_POOL_EXAMPLE] = true;

        // Schedule example?
        else if ( strcmp(argv[i], "schedule") == 0 )

            // Set the schedule example flag
            examples_to_run[PARALLEL_SCHEDULE_EXAMPLE] = true;

        // Default
        else goto invalid_arguments;
    }
    
    // Success
    return;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < PARALLEL_EXAMPLES_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // Success
        return;
    }

    // Error handling
    {

        // Argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}

int parallel_thread_example ( int argc, const char *argv[] )
{
    
    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭────────────────╮\n"\
        "│ thread example │\n"\
        "╰────────────────╯\n"\
        "This example spawns %d threads. Each thread waits for [0, %d] seconds,\n"\
        "and prints a message to standard out. The thread promptly terminates.\n\n",
        PARALLEL_THREADS_QUANTITY,
        PARALLEL_THREADS_MAX_DELAY
    );

    // Initialized data
    parallel_thread *_p_parallel_threads[PARALLEL_THREADS_QUANTITY] = { 0 };

    // Seed the random number generator
    srand(time(NULL));

    // Start some threads
    for (size_t i = 0; i < PARALLEL_THREADS_QUANTITY; i++)

        // Start a thread
        if ( parallel_thread_start(&_p_parallel_threads[i], print_something_to_standard_out, (void *) i) == 0 ) goto failed_to_start_thread;

    // Wait for the threads to finish
    for (size_t i = 0; i < PARALLEL_THREADS_QUANTITY; i++)

        // Wait for the threads to finish
        if ( parallel_thread_join(&_p_parallel_threads[i]) == 0 ) goto failed_to_join_thread;

    // Example formatting
    putchar('\n');

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

    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭─────────────────────╮\n"\
        "│ thread pool example │\n"\
        "╰─────────────────────╯\n"\
        "This example [TODO: Describe example]\n\n"
    );

    // Initialized data
    thread_pool *p_thread_pool = (void *) 0;
    timestamp   *_timestamps[7] = { 0 };
        
    // Construct a thread pool
    //if ( thread_pool_construct(&p_thread_pool, 3) == 0 ) goto failed_to_construct_thread_pool;

    // Enqueue some tasks
    //for (size_t i = 1; i < 3; i++)
    //    thread_pool_run(p_thread_pool, print_something_to_standard_out);

    // Wait for everything to finish
    //thread_pool_wait_idle(p_thread_pool);
    
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

    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭──────────────────╮\n"\
        "│ schedule example │\n"\
        "╰──────────────────╯\n"\
        "In this example, a group of three friends are telling jokes. First Alice, then \n"\
        "Charlie, and finally Bob. Since it's rude to laugh while your friend is telling \n"\
        "a joke, the listeners wait for the punchline before laughing. This demonstrates\n"\
        "the most important features of the scheduler. Concurrency, and parallelism. \n\n"\
    );

    // Initialized data
    parallel_schedule *p_schedule = (void *) 0;
    
    // Register tasks for the scheduler
    (void) parallel_register_task("Alice tells a joke"  , (fn_parallel_task *)alice_joke);
    (void) parallel_register_task("Bob tells a joke"    , (fn_parallel_task *)bob_joke);
    (void) parallel_register_task("Charlie tells a joke", (fn_parallel_task *)charlie_joke);
    (void) parallel_register_task("laugh"               , (fn_parallel_task *)laugh);

    // Construct a schedule
    if ( parallel_schedule_load(&p_schedule, "resources/schedule.json") == 0 ) goto failed_to_construct_schedule;

    // Start the schedule
    if ( parallel_schedule_start(p_schedule, 0) == 0 ) goto failed_to_start_schedule;
    
    // Wait idle
    parallel_schedule_wait_idle(p_schedule);

    // Stop the schedule
    if ( parallel_schedule_stop(p_schedule) == 0 ) goto failed_to_stop_schedule;

    // Destroy the scheudle
    (void) parallel_schedule_destroy(&p_schedule);
    
    // Success
    return 1;

    // Error handling
    {

        // Parallel errors
        {
            failed_to_construct_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to construct schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
            
            failed_to_start_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to start schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            failed_to_stop_schedule:
                #ifndef NDEBUG
                    log_error("[parallel] [schedule] Failed to stop schedule in call to function \"%s\"\n", __FUNCTION__);
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

void *print_something_to_standard_out ( void *p_parameter )
{

    // Initialized data
    int delay = rand() % 5;

    // Sleep for 0-4 seconds
    sleep(delay);

    // Print the parameter to standard out
    printf("Thread %d finished in %d seconds\n", (int) p_parameter, delay); fflush(stdout);

    // Flush standard out
    fflush(stdout);

    // Done
    return 0;
}

void *alice_joke ( void *null_pointer )
{

    // Alice's setup
    printf("Alice > Did you hear the story about the claustrophobic astronaut?\n"); fflush(stdout);
    
    // Alice hesitates
    for (size_t i = 0; i < 3; i++)
    {
        printf("."); fflush(stdout);
        sleep(1);
    }

    // Alice delivers the punchline
    printf("\nAlice > He just needed some space!\n"); fflush(stdout);

    // Success
    return (void *) 1;
}

void *bob_joke ( void *null_pointer )
{

    // Bob's setup
    printf("\nBob > What's red and bad for your teeth?\n"); fflush(stdout);
    
    // Bob hesitates
    for (size_t i = 0; i < 3; i++)
    {
        printf("."); fflush(stdout);
        sleep(1);
    }
    
    // Bob delivers the punchline
    printf("\nBob > A brick!\n"); fflush(stdout);
    
    // Success
    return (void *) 1;
}

void *charlie_joke ( void *null_pointer )
{

    // Charlie's setup
    printf("\nCharlie > What's the leading cause of dry skin?\n"); fflush(stdout);

    // Charlie hesitates
    for (size_t i = 0; i < 3; i++)
    {
        printf("."); fflush(stdout);
        sleep(1);
    }

    // Charlie delivers the punchline
    printf("\nCharlie > Towels!\n"); fflush(stdout);

    // Success
    return (void *) 1;
}

void *laugh ( void *null_pointer )
{

    // Someone is laughing ...
    printf("Hahahaha\n"); fflush(stdout);

    // ... for 1 second
    sleep(1);

    // Success
    return (void *) 1;
}
