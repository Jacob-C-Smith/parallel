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

// Forward declarations
struct parallel_schedule_s;

// Type definitions
typedef struct parallel_schedule_s parallel_schedule;

// Function declarations
/** !
 * Allocate memory for a schedule
 * 
 * @param pp_schedule return
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int parallel_schedule_create ( parallel_schedule **pp_schedule );

DLLEXPORT int parallel_schedule_load ( parallel_schedule **pp_schedule, char *path );

DLLEXPORT int parallel_schedule_load_as_json_text ( parallel_schedule **pp_schedule, char *text );

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
DLLEXPORT int parallel_schedule_load_as_json_value ( parallel_schedule **pp_schedule, json_value *p_value );

/** !
 * Start running a schedule
 *
 * @param p_schedule the schedule
 *
 * @sa stop_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_start ( parallel_schedule *p_schedule );

/** !
 * Stop running a schedule
 *
 * @param p_schedule the schedule
 *
 * @sa start_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int parallel_schedule_stop ( parallel_schedule *p_schedule );

DLLEXPORT int parallel_schedule_destroy ( parallel_schedule **pp_schedule );

