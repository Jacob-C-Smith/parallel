# Need CMake 3.16
cmake_minimum_required (VERSION 3.16.0)

# The name of the repository
project ("parallel")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set the output paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Build sync with semaphore
add_compile_definitions(BUILD_SYNC_WITH_SEMAPHORE)

# Build sync with monitor
add_compile_definitions(BUILD_SYNC_WITH_MONITOR)

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")
    
    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_QUEUE true PARENT_SCOPE)
        set(QUEUE_INCLUDE_DIR ${QUEUE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the array module
if ( NOT "${HAS_JSON}")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has array
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_JSON true PARENT_SCOPE)
        set(JSON_INCLUDE_DIR ${JSON_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_ARRAY true PARENT_SCOPE)
        set(ARRAY_INCLUDE_DIR ${ARRAY_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_DICT true PARENT_SCOPE)
        set(DICT_INCLUDE_DIR ${DICT_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Add the parallel project
if ( NOT "${HAS_PARALLEL}")

    set (PARALLEL_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the log project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_PARALLEL true PARENT_SCOPE)
        set(PARALLEL_INCLUDE_DIR ${PARALLEL_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (parallel_example "main.c")
add_dependencies(parallel_example log json array dict sync)
target_include_directories(parallel_example PUBLIC ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR})
target_link_libraries(parallel_example queue log parallel json array dict sync)


#add_executable (tmp "tmp.c")
#add_dependencies(tmp parallel log json array dict sync)
#target_include_directories(tmp PUBLIC ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR})
#target_link_libraries(tmp queue log parallel json array dict sync)

## Add source to the tester
#add_executable (parallel_test "parallel_test.c")
#add_dependencies(parallel_test array dict sync)
#target_include_directories(parallel_test PUBLIC include include/parallel  ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
#target_link_libraries(parallel_test parallel json array dict sync)

# Add source to this project's library
add_library (parallel SHARED "parallel.c" "thread.c" "schedule.c")
add_dependencies(parallel log json array dict sync)
target_include_directories(parallel PUBLIC ${PARALLEL_INCLUDE_DIR} ${QUEUE_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR})
target_link_libraries(parallel PRIVATE queue log json array dict sync)
