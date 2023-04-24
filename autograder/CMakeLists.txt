cmake_minimum_required(VERSION 3.10.2)
project(prog3_simulacro_PC1_v2023_1_test)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_DEBUG})
if(UNIX AND NOT APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if(MINGW OR CYGWIN)
    add_definitions(-O3)
endif()

# Work Folders
include_directories(.)
include_directories(tests)
include_directories(../tools/catch)

# Project files
file(GLOB project_files
        tests/catch_test_*/*.cpp
        tests/*.h
        tests/*.cpp
        ../src/*.h
        ../src/*.cpp
        )

add_executable(${PROJECT_NAME}
        ${project_files}
    )

if(UNIX AND NOT APPLE)
    find_package(TBB)
    target_link_libraries(${PROJECT_NAME} TBB::tbb)
endif()

# Test files
file(GLOB test_files
        tests/catch_test_*/*.in
        tests/catch_test_*/*.txt
    )
foreach(full_test_file_name ${test_files})
    configure_file(${full_test_file_name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
endforeach(full_test_file_name)