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
 * @return 1 on success, 0 on error
 */
int parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

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

/**!
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );

// Entry point
int main ( int argc, const char *argv[] )
{
    
    // Initialized data
    bool examples_to_run[PARALLEL_EXAMPLES_QUANTITY] = { 0 };

    // Parse command line arguments
    if ( parse_command_line_arguments (argc, argv, &examples_to_run) == 0 )
        
        // Error
        return EXIT_FAILURE;

    // Initialize parallel
    if ( parallel_init() == 0 ) goto failed_to_initialize_parallel;

    // Formatting
    log_info(
        "╭──────────────────╮\n"\
        "│ parallel example │\n"\
        "╰──────────────────╯\n"\
        "The parallel library provides high level abstractions for parallel computing\n"
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
        
        failed_to_run_schedule_example:
            
            // Print an error message
            printf("Failed to run schedule example!\n");

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

int parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
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
    return 1;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < PARALLEL_EXAMPLES_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // Success
        return 1;
    }

    // Error handling
    {

        // Argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Error
                return 0;
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
        "\n"\
        "╭────────────────╮\n"\
        "│ thread example │\n"\
        "╰────────────────╯\n"\
        "A thread is the most primitive abstraction in parallel computing.\n\n"\
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

    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "\n"\
        "╭─────────────────────╮\n"\
        "│ thread pool example │\n"\
        "╰─────────────────────╯\n"\
        "A thread pool is an abstraction for a collection of threads.\n\n"\
        "This example [TODO: Describe example]\n\n"
    );

    // Initialized data
    thread_pool *p_thread_pool = (void *) 0;

    /*
    // Construct a thread pool
    if ( thread_pool_construct(&p_thread_pool, 8, print_something_to_standard_out) == 0 ) goto failed_to_construct_thread_pool;

    // Enqueue some tasks
    // for (size_t i = 1; i < 15; i++) thread_pool_enqueue(p_thread_pool, i);

    p_thread_pool->running = 1;

    sleep(6);
    */
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
        "\n"\
        "╭──────────────────╮\n"\
        "│ schedule example │\n"\
        "╰──────────────────╯\n"\
        "A schedule is an abstraction for coordinating logical units of work over time.\n\n"\
        "This example [TODO: Describe example]\n\n"
    );

    // Initialized data
    parallel_schedule *p_schedule = (void *) 0;
    json_value        *p_value    = (void *) 0;

    // Load and parse the file
    goto load_the_file;
    
    // Done parsing the file
    done_loading_file:
    
    // TODO: Add tasks for scheduler
    //

    // Construct a schedule
    if ( parallel_schedule_load_as_json_value(&p_schedule, p_value) == 0 ) goto failed_to_construct_schedule;

    // Start the schedule
    if ( parallel_schedule_start(p_schedule) == 0 ) goto failed_to_start_schedule;

    // Stop the schedule
    if ( parallel_schedule_stop(p_schedule) == 0 ) goto failed_to_stop_schedule;

    // Destroy the scheudle
    (void) parallel_schedule_destroy(&p_schedule);
    
    // Success
    return 1;

    // This section of code loads the schedule file, and parses it into a json value
    load_the_file:
    {

        // Initialized data
        size_t  file_size       = 0;
        char   *p_file_contents = (void *) 0;
        
        // Query the size of the file
        file_size = load_file("schedule.json", 0, true);

        // Error check
        if ( file_size == 0 ) goto invalid_file;
        
        // Allocate memory for the file
        p_file_contents = PARALLEL_REALLOC(0, file_size);

        // Error check
        if ( p_file_contents == (void *) 0 ) goto no_mem;

        // Load the file
        load_file("schedule.json", p_file_contents, 0);

        // Parse the file into a json value
        if ( parse_json_value(p_file_contents, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

        // Clean up
        PARALLEL_REALLOC(p_file_contents, 0);

        // Done
        goto done_loading_file;
    }

    // Error handling
    {

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

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    if ( path == 0 ) goto no_path;

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[JSON] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

            // Error
            return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

            // Error
            return 0;
        }
    }
}
