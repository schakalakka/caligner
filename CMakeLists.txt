cmake_minimum_required(VERSION 3.6)
project(caligner)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c11 -g -fPIC")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/)

set(SOURCE_FILES src/overlap.c src/overlap.h src/minmax.h src/minmax.c src/alignment.h src/alignment.c)
add_library(aligner SHARED ${SOURCE_FILES})
add_executable(caligner ${SOURCE_FILES} src/main.c)
add_executable(caligner_examples ${SOURCE_FILES} examples/examples.c)