# Need CMake 3.0
cmake_minimum_required (VERSION 3.0)

# The name of the repository
project ("parallel")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Log
    message("[parallel] Adding log project")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    set(HAS_LOG true PARENT_SCOPE)
endif()

# Find the queue module
if ( NOT "${HAS_QUEUE}")
    
    # Log
    message("[parallel] Adding queue project")
    
    # Add the queue directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/queue ${CMAKE_SOURCE_DIR}/build/queue)

    # Let any parent project know that this project has queue
    set(HAS_QUEUE true PARENT_SCOPE)
endif()

# Find the array module
if ( NOT "${HAS_JSON}")
    
    # Log
    message("[parallel] Adding json project")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has array
    set(HAS_JSON true PARENT_SCOPE)
endif()

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Log
    message("[parallel] Adding array project")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Log
    message("[parallel] Adding dict project")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    set(HAS_DICT true PARENT_SCOPE)
endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (parallel_example "main.c")
add_dependencies(parallel_example log crypto json array dict sync)
target_include_directories(parallel_example PUBLIC include include/parallel ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/queue/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/log/include/ ${CMAKE_SOURCE_DIR}/extern/crypto/include/)
target_link_libraries(parallel_example queue log crypto parallel json array dict sync)

## Add source to the tester
#add_executable (parallel_test "parallel_test.c")
#add_dependencies(parallel_test array dict sync)
#target_include_directories(parallel_test PUBLIC include include/parallel  ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
#target_link_libraries(parallel_test parallel json array dict sync)

# Add source to this project's library
add_library (parallel SHARED "parallel.c" "thread.c" "thread_pool.c")
add_dependencies(parallel log crypto json array dict sync)
target_include_directories(parallel PUBLIC include include/parallel ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/queue/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/log/include/ ${CMAKE_SOURCE_DIR}/extern/crypto/include/)
target_link_libraries(parallel PRIVATE queue log crypto json array dict sync)
