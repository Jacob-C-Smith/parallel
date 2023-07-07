/** !
 * @file parallel/parallel.h
 * 
 * @author Jacob C Smith
 *
 * The scheduler specifies and coordinates work over time.
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// sync submodule
#include <sync/sync.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// array submodule
#include <array/array.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Type definitions
/** !
 *  @brief The type definition of a task struct
 */
typedef struct task_s task;

/** !
 *  @brief The type definition of a thread struct
 */
typedef struct thread_s thread;

/** !
 *  @brief The type definition of a thread_pool struct
 */
typedef struct thread_pool_s thread_pool;

/** !
 *  @brief The type definition of a schedule struct
 */
typedef struct schedule_s schedule;

DLLEXPORT int parallel_init ( void );

// Task functions
DLLEXPORT int task_add ( char *name, int(*pfn_task)(void *) );

DLLEXPORT int task_remove ( char *name );

// Initialization
/** !
 *  Initialize the parallel library
 * 
 * @param void
 * 
 * @return 1 on success, 0 on error
*/
int init_parallel ( void );

// Constructors
/** !
 *  Load a schedule from a JSON file
 *
 * @param pp_schedule : return
 * @param path        : The path to a JSON file containing an schedule object
 *
 * @sa load_schedule_as_json_text
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int schedule_load ( schedule **pp_schedule, char *path );

/** !
 *  Load a schedule from JSON text
 *
 * @param pp_schedule : return
 * @param text        : The schedule JSON object text
 *
 * @sa load_schedule
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int schedule_load_as_json_text ( schedule **pp_schedule, char *text );

/** !
 *  Load a schedule from a JSON value
 *
 * @param pp_schedule : return
 * @param p_value     : The schedule JSON object value
 *
 * @sa load_schedule
 * @sa load_schedule_as_json_text
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int schedule_load_as_json_value ( schedule **pp_schedule, JSONValue_t *p_value );

/** !
 *  Load a thread from a JSON file
 *
 * @param pp_thread : return
 * @param path      : The path to a JSON file containing an schedule object
 *
 * @sa load_schedule_as_json_value
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_load ( thread **pp_thread, char *path );

/** !
 *  Load a thread from a JSON file
 *
 * @param pp_thread : return
 * @param text      : The thread JSON object text
 *
 * @sa load_thread_as_json_text
 * @sa create_thread
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_load_as_json_text ( thread **pp_thread , char *text );

// Scheduling
/** !
 *  Start running a schedule
 *
 * @param p_schedule         : Pointer to a schedule
 * @param p_time             : return the time in seconds
 * @param p_thread_parameter : This is passed directly to each constructed thread
 * 
 * @sa stop_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int schedule_start ( schedule *p_schedule, double *p_time, void *p_thread_parameter );

/** !
 *  Stop running a schedule
 *
 * @param p_schedule  : Pointer to a schedule
 *
 * @sa start_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int schedule_stop ( schedule *p_schedule );

// Destructors
/** !
 *  Destroy a schedule
 *
 * @param pp_schedule : Pointer to a schedule
 *
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int schedule_destroy ( schedule **pp_schedule );

/** !
 *  Destroy a thread
 *
 * @param pp_thread : Pointer to a thread
 *
 * @sa create_thread
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int thread_destroy ( thread **pp_thread );
