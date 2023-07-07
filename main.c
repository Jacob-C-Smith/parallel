// Standard library
#include <stdio.h>
#include <stdlib.h>

// parallel submodule
#include <parallel/parallel.h>

int task_1 ( void* vp_ );
int task_2 ( void* vp_ );
int task_3 ( void* vp_ );

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    schedule *p_schedule_no_order = 0,
             *p_schedule_ordered  = 0;

    // Initialize parallel
    parallel_init();

    // Supply tasks
    task_add("task 1", &task_1);
    task_add("task 2", &task_2);
    task_add("task 3", &task_3);

    // Load the schedules
    if ( schedule_load(&p_schedule_no_order, "schedule unordered.json") == 0 ) goto failed_to_load_schedule;
    if ( schedule_load(&p_schedule_ordered , "schedule ordered.json")   == 0 ) goto failed_to_load_schedule;

    // Start the unordered schedule. 
    if ( schedule_start(p_schedule_no_order, 0, 0) == 0 ) goto failed_to_start_schedule;

    // Start the ordered schedule
    if ( schedule_start(p_schedule_no_order, 0, 0) == 0 ) goto failed_to_start_schedule;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        
        failed_to_load_schedule:

            // Print an error message
            printf("Failed to load schedule!\n");

            // Error
            return 0;

        failed_to_start_schedule:

            // Print an error message
            printf("Failed to start schedule!\n");

            // Error
            return 0;
    }
}

int task_1 ( void* vp_  )
{
    printf("TASK 1\n");

    return 1;
}

int task_2 ( void* vp_  )
{
    printf("TASK 2\n");

    return 1;
}

int task_3 ( void* vp_  )
{
    printf("TASK 3\n");

    return 1;
}