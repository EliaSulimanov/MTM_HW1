cmake_minimum_required(VERSION 3.0.0)

project(hw1 VERSION 0.1.0 LANGUAGES C)

set(CMAKE_VERBOSE_MAKEFILE ON)

#set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror -g -L. -lmap")
set(MTM_FLAGS_DEBUG "-std=c99 --pedantic-errors -Wall -Werror -g")
set(MTM_FLAGS_RELEASE "${MTM_FLAGS_DEBUG} -DNDEBUG")

set(CMAKE_C_FLAGS ${MTM_FLAGS_DEBUG})

#link_directories(.)

add_executable(hw1 main.c mtm_map/map.c mtm_map/node.c election.c)

#target_link_libraries(hw1 libmap.a)