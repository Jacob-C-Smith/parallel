# Need CMake 3.26
cmake_minimum_required (VERSION 3.26)

# The name of the repository
project ("parallel")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)



if ( NOT "${HAS_JSON}")
    message("[parallel] Adding json project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)
    set(HAS_JSON CACHE true PARENT_SCOPE)
endif()

if ( NOT "${HAS_ARRAY}")
    message("[parallel] Adding array project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)
    set(HAS_ARRAY CACHE true PARENT_SCOPE)
endif()

if ( NOT "${HAS_DICT}")
    message("[parallel] Adding dict project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)
    set(HAS_DICT CACHE true PARENT_SCOPE)
endif()

#if ( NOT "${HAS_SYNC}")
#    message("[parallel] Adding sync project")
#    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)
#    set(HAS_SYNC CACHE true PARENT_SCOPE)
#endif()

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (parallel_example "main.c")
add_dependencies(parallel_example json array dict sync)
target_include_directories(parallel_example PUBLIC include include/parallel  ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(parallel_example parallel json array dict sync)

## Add source to the tester
#add_executable (parallel_test "parallel_test.c")
#add_dependencies(parallel_test array dict sync)
#target_include_directories(parallel_test PUBLIC include include/parallel  ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
#target_link_libraries(parallel_test parallel json array dict sync)

# Add source to this project's library
add_library (parallel SHARED "parallel.c")
add_dependencies(parallel json array dict sync)
target_include_directories(parallel PUBLIC include include/parallel  ${CMAKE_SOURCE_DIR}/extern/parallel/include ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(parallel PRIVATE json array dict sync)
