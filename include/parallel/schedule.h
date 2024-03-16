/** !
 * High level abstractions for schedule
 * 
 * @file parallel/schedule.h
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// sync 
#include <sync/sync.h>

// dict
#include <dict/dict.h>

// json
#include <json/json.h>

// parallel
#include <parallel/parallel.h>
#include <parallel/thread.h>

// Forward declarations
struct parallel_schedule_s;

// Type definitions
typedef struct parallel_schedule_s parallel_schedule;
typedef void (fn_scheduler_task)(void *p_parameter);

// Function declarations
/** !
 * Allocate memory for a schedule
 * 
 * @param pp_schedule return
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int parallel_schedule_create ( parallel_schedule **const pp_schedule );

/** !
 * Construct a schedule from a file 
 * 
 * @param pp_schedule return
 * @param path        path to the file
 * 
 * @return 1 on success, 0 on error 
*/
DLLEXPORT int parallel_schedule_load ( parallel_schedule **const pp_schedule, const char *const path );

/** !
 * Construct a schedule from json text 
 * 
 * @param pp_schedule return
 * @param text        the json text
 * 
 * @return 1 on success, 0 on error 
*/
DLLEXPORT int parallel_schedule_load_as_json_text ( parallel_schedule **const pp_schedule, const char *const text );

/** !
 * Construct a schedule from a json value
 * 
 * @param pp_schedule return
 * @param p_value     the json value
 * 
 * @sa parallel_schedule_load
 * @sa parallel_schedule_load_as_json_text
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int parallel_schedule_load_as_json_value ( parallel_schedule **const pp_schedule, const json_value *const p_value );

/** !
 * Start running a schedule
 *
 * @param p_schedule the schedule
 *
 * @sa stop_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_start ( parallel_schedule *const p_schedule );

/** !
 * Stop running a schedule
 *
 * @param p_schedule the schedule
 *
 * @sa start_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_stop ( parallel_schedule *const p_schedule );

/** !
 * Register a task with the scheduler
 * 
 * @param name     the name of the task
 * @param pfn_task pointer to task function
 * 
 * @sa parallel_schedule_unregister_task
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_register_task ( const char *const name, fn_scheduler_task *pfn_scheduler_task );

/** !
 * Unegister a task from the scheduler
 * 
 * @param name the name of the task
 * 
 * @sa parallel_schedule_register_task
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_unrgister_task ( const char *const name );

/** !
 * Destroy a schedule
 * 
 * @param pp_schedule the schedule
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_destroy ( parallel_schedule **const pp_schedule );

