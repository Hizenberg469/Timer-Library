cmake_minimum_required(VERSION 3.22.1)

project(Timer-Library VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD          17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS        OFF)


#Output file structure

#Scheme 1
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")

#Scheme 2
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>/lib")

#Library Name...
set(TIMER_LIB timer)
set(WHEELTIMER_LIB wheeltimer)


#Header file directory...
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

#Source file directory...
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

#Adding Sub-directory having CMakeLists.txt...
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(Assignment)