#timerlib.c src file...
set(TIMER_LIB_SRC "timerlib.c")
set(WHEEL_TIMER_LIB_SRC "WheelTimer.c")
set(GLTHREAD "glthread.c")

#Library
add_library(${TIMER_LIB} STATIC
				${TIMER_LIB_SRC})

add_library(${WHEELTIMER_LIB} STATIC
						${TIMER_LIB_SRC}
						${GLTHREAD}
						${WHEEL_TIMER_LIB_SRC})



#Linking dependent library...

if( NOT (CMAKE_SYSTEM_NAME STREQUAL "Linux") )
	message(FATAL_ERROR "It's not a Unix-based system.\n \
	POSIX TIMER Library will not compile in this project.\n")
endif()

set(POSIX_TIMER_LIB rt)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#Below snippet is important for POSIX timer library in WSL2...
target_compile_definitions(wheeltimer PRIVATE _POSIX_C_SOURCE=199309L)
target_link_libraries(${TIMER_LIB} PUBLIC
							${POSIX_TIMER_LIB}
							Threads::Threads)

target_link_libraries(${WHEELTIMER_LIB} PUBLIC
							${POSIX_TIMER_LIB}
							Threads::Threads)

#Including Header file dir...
target_include_directories(${TIMER_LIB} PUBLIC
								${HEADER_DIR})

target_include_directories(${WHEELTIMER_LIB} PUBLIC
								${HEADER_DIR})