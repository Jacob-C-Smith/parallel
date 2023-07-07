// parallel header
#include <parallel/parallel.h>

// Structure definitions
struct task_s
{

	// The wait_for struct describes what thread task needs to finish before
	// starting this task.
    struct
    {
        char *thread, // The thread where the task takes place
             *task;   // The task that must complete before this task starts
    } wait_for;

	// The function struct holds a function pointer and a function name
    struct
    {
        char  *name;                          // The name of the task
        int  (*pfn_function_pointer)(void *); // A pointer to the task function
    } function;
};

struct thread_s
{
	char *name;    // The name of the thread
	bool  running; // Is the thread running?

	// The tasks struct describes an ordered list of tasks to be run, and 
	// stores state about completed tasks. This completion state may be 
	// reset if the thread is associated with a thread pool / schedule.
    struct
    {
	    size_t   task_count,       // The quantity of tasks this thread will complete
		        *p_complete_tasks; // An array of "task_count" size. 1 is done, 0 is incomplete
    	task   **pp_tasks;         // An array of pointers to tasks
    } tasks;

	//SDL_Thread  *thread;
};

// TODO
struct thread_pool_s
{
	// TODO:
	// STUB
	int i;
};

struct schedule_s
{
	char *name;    // The name of the schedule
	dict *data;    // The threads of the schedule
	bool  running; // Is the schedule running?
};

// This dict records task names and function pointers
static dict *tasks_lookup_table = 0;

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    {
        #ifndef NDEBUG
            if ( path == 0 )
                goto no_path;
        #endif
    }

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL )
        goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer )
        ret = fread(buffer, 1, ret, f);

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

// Initialize the parallel library
int parallel_init ( void )
{

	// Construct a task lookup table
	if ( dict_construct(&tasks_lookup_table, 128) == 0 ) goto failed_to_construct_scheduler_tasks_lookup_table;

	// Success
	return 1;

	// Error handling
	{

		// G10 errors
		{
			failed_to_construct_scheduler_tasks_lookup_table:
				#ifndef NDEBUG
					printf("[parallel] Failed to construct task lookup table in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int task_create ( task **pp_task )
{

	// Argument check
    #ifndef NDEBUG
	    if ( pp_task == (void *) 0 ) goto no_task;
	#endif

	// Initialized data
	task *p_task = calloc(1, sizeof(task));

	// Error check
	if ( p_task == (void *) 0 ) goto no_mem;

	// Return a pointer to the caller
	*pp_task = p_task;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_task:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"pp_task\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int thread_create ( thread **pp_thread )
{

	// Argument check
	#ifndef NDEBUG
	    if ( pp_thread == (void *) 0 ) goto no_thread;
    #endif

	// Initialized data
	thread *p_thread = calloc(1, sizeof(thread));

	// Error checking
	if ( p_thread == (void *) 0 ) goto no_mem;

	// Return a pointer to the caller
	*pp_thread = p_thread;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_thread:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int thread_pool_create ( thread_pool **pp_thread_pool )
{

	// Argument check
	#ifndef NDEBUG
	    if ( pp_thread_pool == (void *) 0 ) goto no_thread_pool;
    #endif

	// Initialized data
	thread_pool *p_thread_pool = calloc(1, sizeof(thread_pool));

	// Error checking
	if ( p_thread_pool == (void *) 0 ) goto no_mem;

	// Return a pointer to the caller
	*pp_thread_pool = p_thread_pool;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_thread_pool:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"pp_thread_pool\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int schedule_create ( schedule **pp_schedule )
{

	// Argument check
	#ifndef NDEBUG
	    if ( pp_schedule == (void *) 0 ) goto no_schedule;
    #endif

	// Initialized data
	schedule *p_scheduler = calloc(1, sizeof(schedule));

	// Error checking
	if ( p_scheduler == (void *) 0 ) goto no_mem;

	// Return a pointer to the caller
	*pp_schedule = p_scheduler;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int task_add ( char *name, int(*pfn_task)(void *) )
{

    // Argument check
    #ifndef NDEBUG
        if ( name     == (void *) 0 ) goto no_name;
        if ( pfn_task == (void *) 0 ) goto no_task;
    #endif

	// Add the task to the lookup
	(void) dict_add(tasks_lookup_table, name, (void *)pfn_task);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_name:
                #ifndef NDEBUG
                    printf("[parallel] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_task:
                #ifndef NDEBUG
                    printf("[parallel] Null pointer provided for parameter \"task\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int task_remove ( char *name )
{

    // Argument check
    #ifndef NDEBUG
        if ( name     == (void *) 0 ) goto no_name;
    #endif

	// Remove the task from the lookup
	(void) dict_pop(tasks_lookup_table, name, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_name:
                #ifndef NDEBUG
                    printf("[parallel] Null pointer provided for parameter \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_task:
                #ifndef NDEBUG
                    printf("[parallel] Null pointer provided for parameter \"task\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int task_load_as_json_value ( task **pp_task, JSONValue_t *p_value )
{

	// Argument check
    #ifndef NDEBUG
	    if ( pp_task == (void *) 0 ) goto no_task;
		if ( p_value == (void *) 0 ) goto no_value;
	#endif

	// Initialized data
	task *p_task      = 0;
	char *task_name   = 0,
		 *wait_thread = 0,
		 *wait_task   = 0;

	// Allocate memory for the task struct
	if ( task_create(&p_task) == 0 ) goto failed_to_create_task;

	// TODO: Refactor to use JSONValue_t *
	// Parse the task JSON
	if ( p_value->type == JSONobject )
	{

		// Initialized data
		dict *p_dict = p_value->object;
		
		// Parse the JSON values into constructor parameters
		task_name   = ((JSONValue_t *)dict_get(p_dict, "task"))->string;

		if ( dict_get(p_dict, "wait thread") )
			wait_thread = ((JSONValue_t *)dict_get(p_dict, "wait thread"))->string;

		if ( dict_get(p_dict, "wait thread") )
			wait_task   = ((JSONValue_t *)dict_get(p_dict, "wait task"))->string;

		// Error checking
		if ( ( task_name ) == 0 ) goto missing_properties;
	}
	// Default
	else
		goto wrong_type;

	// Construct the task
	{

		// Copy the name
		{

			// Initialized data
			size_t task_name_len = strlen(task_name);

			// Allocate memory for the name
			p_task->function.name = calloc(task_name_len+1, sizeof(char));

			// Error handling
			if ( p_task->function.name == 0 ) goto no_mem;

			// Copy the name
			strncpy(p_task->function.name, task_name, task_name_len);
		}

		// Copy the wait thread
		if ( wait_thread )
		{

			// Initialized data
			size_t wait_thread_name_len = strlen(wait_thread);

			// Allocate memory for the name
			p_task->wait_for.thread = calloc(wait_thread_name_len+1, sizeof(char));

			// Error handling
			if ( p_task->wait_for.thread == 0 ) goto no_mem;

			// Copy the name
			strncpy(p_task->wait_for.thread, wait_thread, wait_thread_name_len);
		}

		// Copy the wait task
		if ( wait_task )
		{

			// Initialized data
			size_t wait_task_name_len = strlen(wait_task);

			// Allocate memory for the name
			p_task->wait_for.task = calloc(wait_task_name_len+1, sizeof(char));

			// Error handling
			if ( p_task->function.name == 0 ) goto no_mem;

			// Copy the name
			strncpy(p_task->wait_for.task, wait_task, wait_task_name_len);
		}
	}

	// Get a function pointer from the list
	p_task->function.pfn_function_pointer = (int (*)(void *)) dict_get(tasks_lookup_table, p_task->function.name);

	// Get a pointer to the allocated memory
	*pp_task = p_task;

	// Success
	return 1;

	// TODO: 
	no_task:
	no_value:
	failed_to_create_task:
		return 0;

	// Error handling
	{
		
		// JSON parsing errors
		{
			wrong_type:
				#ifndef NDEBUG
					printf("[parallel] Expected a JSON object in call to function \"%s\"\n Refer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			missing_properties:
				#ifndef NDEBUG
					printf("[parallel] Not enough properties to construct task in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int thread_load_as_json_value ( thread **pp_thread, JSONValue_t *p_value )
{

	// Argument check
    #ifndef NDEBUG
		if ( pp_thread == (void *) 0 ) goto no_thread;
	    if ( p_value   == (void *) 0 ) goto no_value;
    #endif

	// Initialized data
	thread  *p_thread    = 0;
	char    *name        = 0,
	        *description = 0;
	size_t   task_count  = 0;
	array   *p_tasks     = 0;

	// TODO: Refactor to use JSONValue_t *
	// Parse the thread JSON
	if ( p_value->type == JSONobject )
	{

		// Initialized data
		dict *p_dict = p_value->object;

		// Parse the JSON values into constructor parameters
		name        = ((JSONValue_t *)dict_get(p_dict, "name"))->string;
		description = ((JSONValue_t *)dict_get(p_dict, "description"))->string;
		p_tasks     = ((JSONValue_t *)dict_get(p_dict, "tasks"))->list;

		// Error checking
		if ( ( name && description && p_tasks ) == 0 ) goto missing_properties;
	}
	else
		goto wrong_type;

	// Construct the thread
	{

		// Initialized data
		thread *p_thread       = 0;
		char       *p_name         = 0;
		size_t     *complete_tasks = 0;
		dict       *d_tasks        = 0;
		task  **tasks          = calloc(task_count+1, sizeof(task *));

		// Copy the thread name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for the name
			p_name = calloc(name_len+1, sizeof(char));

			// Error checking
			if ( p_name == (void *) 0 ) goto no_mem;

			// Copy the name
			strncpy(p_name, name, name_len);

		}

		// Construct thread tasks
		{

			// Initialized data
			JSONValue_t **pp_elements  = 0;

			// Dump the array contents
			{

				// Get the quantity of elements
				array_get(p_tasks, 0, &task_count);

				// Allocate an array for the elements
				pp_elements = calloc(task_count+1, sizeof(JSONValue_t *));

				// Error check
				if ( pp_elements == (void *) 0 ) goto no_mem;

				// Populate the elements of the tasks
				array_get(p_tasks, (void **)pp_elements, 0 );
			}
			
			// Construct a task dict
			dict_construct(&d_tasks, task_count);

			// Construct a list of tasks
			tasks = calloc(task_count+1, sizeof(task *));

			// Construct a list of size_t's
			complete_tasks = calloc(task_count+1, sizeof(size_t));

			// Error check
			if ( complete_tasks == (void *) 0 ) goto no_mem;
			if ( tasks == (void *) 0 ) goto no_mem;

			// Set up the tasks
			for (size_t i = 0; i < task_count; i++)
			{

				// Initialized data
				JSONValue_t *p_thread_json_value = pp_elements[i];
				task    *p_task              = 0;

				// Type check
				if ( p_thread_json_value->type != JSONobject ) return 0; // TODO: Error handling

				// Load the thread
				if ( task_load_as_json_value(&p_task, p_thread_json_value) == 0 ) goto failed_to_load_thread;

				// Add the thread to the thread dict
				dict_add(d_tasks, p_task->function.name, p_task);

				// Set the task in the list
				tasks[i] = p_task;

			}

			// Clean the scope
			free(pp_elements);
		}

		// Allocate the thread
		if ( thread_create(&p_thread) == 0 ) goto failed_to_create_thread;

		// Populate the thread struct
		*p_thread = (thread)
		{
			.name = p_name,
			.tasks =
			{
				.p_complete_tasks = complete_tasks,
				.pp_tasks = tasks,
				.task_count = task_count
			},
			.running = false
		};

        // Return a pointer to the caller
		*pp_thread = p_thread;
	}

	// Success
	return 1;

	// Error handling
	{

		// JSON parsing errors
		{
			failed_to_parse_json:
				#ifndef NDEBUG
					printf("[parallel] Failed to parse JSON in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			wrong_type:
				#ifndef NDEBUG
					printf("[parallel] Expected a JSON object in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			missing_properties:
				#ifndef NDEBUG
					printf("[parallel] Not enough properties to construct thread in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 Errors
		{

			failed_to_create_schedule:
				#ifndef NDEBUG
					printf("[parallel] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_thread:
				#ifndef NDEBUG
					printf("[parallel] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_create_thread:
				#ifndef NDEBUG
					printf("[parallel] Failed to create thread in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Argument errors
		{
			no_thread:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		no_value:
		return 0;
	}

}

int schedule_load ( schedule **pp_schedule, char* path )
{

	// Argument check
    #ifndef NDEBUG
    	if ( pp_schedule == (void *) 0 ) goto no_schedule;
    	if ( path        == (void *) 0 ) goto no_path;
    #endif

	// Initialized data
	size_t  len  = load_file(path, 0, true);
	char   *text = calloc(len+1, sizeof(char));

	// Error checking
	if ( text == (void *) 0 ) goto no_mem;

	// Load the file
	if ( load_file(path, text, true) == 0) goto failed_to_load_file;

	// Construct the schedule from the file contents
	if ( schedule_load_as_json_text(pp_schedule, text) == 0 ) goto failed_to_load_schedule;

	// Free the file contents
	free(text);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_path:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 Errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					printf("[parallel] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_schedule:
				#ifndef NDEBUG
					printf("[parallel] Failed to load scheduler from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int schedule_load_as_json_text ( schedule **pp_schedule, char *text )
{

	// Argument check
    #ifndef NDEBUG
    	if ( pp_schedule == (void *) 0 ) goto no_schedule;
    	if ( text        == (void *) 0 ) goto no_text;
    #endif

	// Initialized data
	char        *name          = 0;
	array       *p_threads     = 0;
	JSONValue_t *p_value       = 0;

	// Parse the text into a JSON value
	if ( parse_json_value(text, 0, &p_value) == 0 ) goto failed_to_parse_json;

	// Load the schedule as a JSON value
	if ( schedule_load_as_json_value(pp_schedule, p_value) == 0 ) goto failed_to_create_schedule;

	// Deallocate the JSON value
	free_json_value(p_value);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 Errors
		{
			failed_to_create_schedule:
				#ifndef NDEBUG
					printf("[parallel] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_thread:
				#ifndef NDEBUG
					printf("[parallel] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// JSON errors
		{
			failed_to_parse_json:
				#ifndef NDEBUG
					printf("[parallel] Failed to parse JSON in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int schedule_load_as_json_value ( schedule **pp_schedule, JSONValue_t *p_value )
{

	// Argument check
    #ifndef NDEBUG
    	if ( pp_schedule == (void *) 0 ) goto no_schedule;
    	if ( p_value     == (void *) 0 ) goto no_value;
    #endif

	// Initialized data
	JSONValue_t  *p_name        = 0,
	             *p_threads     = 0;

	// Parse the schedule as an object
	if ( p_value->type == JSONobject )
	{

		// Initialized data
		dict *p_dict = p_value->object;

		// Parse the JSON values into constructor parameters
		p_name    = dict_get(p_dict, "name");
		p_threads = dict_get(p_dict, "threads");

		// Error checking
		if ( ! ( 
			p_name    &&
			p_threads 
		) )
			goto missing_properties;
	}
	// Parse the schedule as a path
	else if ( p_value->type == JSONstring )
	{

		// Load the schedule from the path
		if ( schedule_load(pp_schedule, p_value->string) == 0 )
			goto failed_to_load_schedule;

		// Success
		return 1;
	}
	// Default
	else
		goto wrong_type;

	// Construct the schedule
	{

		// Initialized data
		schedule *p_schedule = 0;
		dict     *threads    = 0;
		char     *name       = 0;

		// Copy the schedule name
		if ( p_name->type == JSONstring )
		{

			// Initialized data
			size_t name_len = strlen(p_name->string);

			// Error checking
			if ( name_len > 255 ) goto name_too_long;

			// Allocate for the name
			name = calloc(name_len+1, sizeof(char));

			// Error checking
			if ( name == (void *) 0 ) goto no_mem;

			// Copy the name
			strncpy(name, p_name->string, name_len);
		}

		// Construct schedule threads
		{

			// Initialized data
			JSONValue_t **pp_elements  = 0;
			size_t        thread_count = 0;

			// Get the array contents
			{

				// Get the quantity of elements
				array_get(p_threads->list, 0, &thread_count);

				// Allocate an array for the elements
				pp_elements = calloc(thread_count+1, sizeof(JSONValue_t *));

				// Error check
				if ( pp_elements == (void *) 0 ) goto no_mem;

				// Populate the elements of the threads
				array_get(p_threads->list, (void **)pp_elements, 0 );
			}

			// Construct a thread dict
			dict_construct(&threads, thread_count);

			// Set up the threads
			for (size_t i = 0; i < thread_count; i++)
			{

				// Initialized data
				JSONValue_t *p_thread_json_value = pp_elements[i];
				thread  *p_thread            = 0;

				// Type check
				if ( p_thread_json_value->type != JSONobject ) goto wrong_thread_type;

				// Load the thread
				if ( thread_load_as_json_value(&p_thread, p_thread_json_value) == 0 ) goto failed_to_load_thread;

				// Add the thread to the thread dict
				dict_add(threads, p_thread->name, p_thread);
			}

			// Clean the scope
			free(pp_elements);
		}

		// Allocate the schedule
		if ( schedule_create(&p_schedule) == 0 ) goto failed_to_create_schedule;

		// Construct the schedule
		*p_schedule = (schedule)
		{
			.name = name,
			.data = threads
		};

		// Return the schedule to the caller
		*pp_schedule = p_schedule;
	}

	// Success
	return 1;

	// Error handling
	{

		// JSON parsing errors
		{
			failed_to_parse_json:
				#ifndef NDEBUG
					printf("[parallel] Failed to parse JSON in call to function \"%s\"\nRefer to schedule schema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			wrong_type:
				#ifndef NDEBUG
					printf("[parallel] Expected a JSON object in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			name_too_long:
				#ifndef NDEBUG
					printf("[parallel] \"name\" property's length must be less than 256 in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			missing_properties:
				#ifndef NDEBUG
					printf("[parallel] Not enough properties to construct schedule in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;

			wrong_thread_type:
				#ifndef NDEBUG
					printf("[G10] Element in property \"threads\" must be of type [ object ] in call to function \"%s\"\nRefer to gschema: https://schema.g10.app/schedule.json \n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 Errors
		{

			failed_to_create_schedule:
				#ifndef NDEBUG
					printf("[parallel] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_thread:
				#ifndef NDEBUG
					printf("[parallel] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_schedule:
				#ifndef NDEBUG
					printf("[parallel] Failed to load scheduler as path in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_value:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int work ( schedule *p_schedule, void *vp_thread, void *thread_parameter )
{

	// Argument check
	#ifndef NDEBUG
        if ( vp_thread == (void *) 0 ) goto no_thread;
    #endif

	// Initialized data
	thread  *p_thread = vp_thread;
	task   **tasks    = p_thread->tasks.pp_tasks;

	// Run until told otherwise
	while ( p_thread->running )
	{

		// Iterate over each task
		for (size_t i = 0; i < p_thread->tasks.task_count; i++)
		{
			
			// Initialized data
			int (*function_pointer)(void *) = 0;
			
			// Is the program waiting on anything else to finish?
			if ( tasks[i]->wait_for.thread )
			{

				// Initialized data
				thread *wait_thread = (thread *) dict_get(p_schedule->data, p_thread->tasks.pp_tasks[i]->wait_for.thread);
				task   *wait_task   = 0;
				size_t  v           = 0;

				// Figure out what task the program is waiting on
				for (size_t j = 0; j < wait_thread->tasks.task_count; j++)
				{

					// Compare 
					if ( strcmp(p_thread->tasks.pp_tasks[i]->wait_for.task, wait_thread->tasks.pp_tasks[j]->function.name) == 0 )
						v = j;
				}

				// Wait for the task to finish
				while (wait_thread->tasks.p_complete_tasks[i] == 0);
			}
			
			// Get the task function
			function_pointer = p_thread->tasks.pp_tasks[i]->function.pfn_function_pointer;
			
			// Run the function
			if ( function_pointer )
				function_pointer( thread_parameter );
			else
				goto no_function_pointer;				

			// Update the task
			p_thread->tasks.p_complete_tasks[i] = 1;
		}

		// Reset
		for (size_t i = 0; i < p_thread->tasks.task_count; i++)
			p_thread->tasks.p_complete_tasks[i] = 0;
	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_thread:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"vp_thread\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// parallel errors
		{
			no_function_pointer:
				#ifndef NDEBUG
					printf("[parallel] Task was not implemented in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int main_work ( schedule *p_schedule, thread *p_thread, void *thread_parameter )
{

	// Argument check
	#ifndef NDEBUG
	    if ( p_thread == (void *) 0 ) goto no_thread;
    #endif

	// Initialized data
	task **tasks = p_thread->tasks.pp_tasks;

	// Run until told otherwise
	while ( p_thread->running )
	{

		// Iterate over each task
		for (size_t i = 0; i < p_thread->tasks.task_count; i++)
		{

			// Initialized data
			int (*function_pointer)(void*) = 0;

			// Is the program waiting on anything else to finish?
			if (p_thread->tasks.pp_tasks[i]->wait_for.thread)
			{

				// Initialized data
				thread *wait_thread = (thread *) dict_get(p_schedule->data, p_thread->tasks.pp_tasks[i]->wait_for.thread);
				task   *wait_task   = 0;
				size_t  v           = 0;

				// Figure out what task the program is waiting on
				for (size_t j = 0; j < wait_thread->tasks.task_count; j++)
				{
					if ( strcmp(p_thread->tasks.pp_tasks[i]->wait_for.task, wait_thread->tasks.pp_tasks[j]->function.name) == 0 )
						v = j;
				}

				// Wait for the task to finish
				while (wait_thread->tasks.p_complete_tasks[i] == 0);

			}

			function_pointer = p_thread->tasks.pp_tasks[i]->function.pfn_function_pointer;

			// Run the function
			if ( function_pointer )
				function_pointer( thread_parameter );

			// Update the task
			p_thread->tasks.p_complete_tasks[i] = 1;
		}

		// Reset
		for (size_t i = 0; i < p_thread->tasks.task_count; i++)
			p_thread->tasks.p_complete_tasks[i] = 0;

	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_thread:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"p_thread\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int schedule_start ( schedule *p_schedule, double *p_time, void *thread_parameter )
{

	// Argument check
    #ifndef NDEBUG
	    if ( p_schedule == (void *) 0 ) goto no_schedule;
	#endif

	// Initialized data
	size_t    schedule_thread_count = dict_values(p_schedule->data, 0);
	thread  **schedule_threads      = calloc(schedule_thread_count, sizeof(void *));
	thread   *main_thread           = 0;

	// TODO: Error check
	// TODO: Zero memory

	// Get a list of threads
	dict_values(p_schedule->data, (void **)schedule_threads);

	// Iterate over each thread
	for (size_t i = 0; i < schedule_thread_count; i++)
	{

		// Initialized data
		thread *thread = schedule_threads[i];

		// Don't spawn a new thread for the main thread
		// TODO: user defined main thread name
		if ( strcmp(thread->name, "Main Thread") == 0 ) continue;

		// Set the thread to run
		thread->running = true;

		// TODO: Implement this
		// Create the thread
		// thread->thread  = SDL_CreateThread(work, thread->name, thread);
	}

	// Get the main thread
	// TODO: user defined main thread name
	main_thread = (thread *) dict_get(p_schedule->data, "Main Thread");

	// Set the thread to run
	main_thread->running = true;

	// Call the thread worker function (on the main thread...)
	main_work(p_schedule, main_thread, thread_parameter);

	// Success
	return 1;

	// Error handling
	{

		// Argument error
		{
			no_schedule:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int schedule_stop ( schedule *p_schedule )
{

	// Argument check
    #ifndef NDEBUG
	    if ( p_schedule == (void *) 0 ) goto no_schedule;
	#endif

	// Initialized data
	size_t    schedule_thread_count = dict_values(p_schedule->data, 0);
	int       r_stat                = 0;
	thread  **schedule_threads      = calloc(schedule_thread_count+1, sizeof(void *));

	// Get a list of threads
	dict_values(p_schedule->data, (void **)schedule_threads);

	// Iterate over each thread
	for (size_t i = 0; i < schedule_thread_count; i++)
	{

		// Initialized data
		thread* thread = schedule_threads[i];

		// Stop the thrad
		thread->running = false;

		// Wrap up every task
		for (size_t i = 0; i < thread->tasks.task_count; i++)
			thread->tasks.p_complete_tasks[i] = 1;
	}

	// Iterate over each thread
	for (size_t i = 0; i < schedule_thread_count; i++)

		// Wait for each thread to finish
		// TODO: implement
		;//SDL_WaitThread(schedule_threads[i]->thread, &r_stat);

	// Success
	return 1;

    // Error handling
	{

		// Argument error
		{
			no_schedule:
				#ifndef NDEBUG
					printf("[parallel] Null pointer provided for parameter \"p_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int schedule_wait ( schedule *p_schedule )
{

	// Argument check
	#ifndef NDEBUG
		if ( p_schedule == (void *) 0 ) goto no_schedule;
	#endif

	// Wait for the thread to end
	while ( p_schedule->running );

	// Success
	return 1;

	// Error handling
	{
		no_schedule:
			#ifndef NDEBUG
				printf("[parallel] Null pointer provided for function \"%s\"\n", __FUNCTION__);
			#endif

			// Error
			return 0;
	}
}

int task_destroy ( task **pp_task )
{

    // Success
    return 1;
}

int thread_destroy ( thread **pp_thread )
{

    // Success
    return 1;
}

int schedule_destroy ( schedule **pp_schedule )
{

	// TODO: Instead of destroyign a schedule, put the schedule in a backlog of 128(?) schedules to be destroyed. 

    // Success
    return 1;
}