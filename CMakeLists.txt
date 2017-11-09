cmake_minimum_required(VERSION 3.8)
project(Shell_Os2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c commands.c command_parser.c command_parser.h commands.h file_processing.c file_processing.h variables.c variables.h stringOperations.c)
add_executable(Shell_Os2 ${SOURCE_FILES})