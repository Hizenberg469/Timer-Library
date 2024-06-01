#timerlib.c src file...
set(TIMER_LIB_SRC "timerlib.c")

#Library
add_library(${TIMER_LIB} STATIC
				${TIMER_LIB_SRC})

#Including Header file dir...
target_include_directories(${TIMER_LIB} PUBLIC
								${HEADER_DIR})

#Linking dependent library...

if( NOT (CMAKE_SYSTEM_NAME STREQUAL "Linux") )
	message(FATAL_ERROR "It's not a Unix-based system.\n \
	POSIX TIMER Library will not compile in this project.\n")
endif()

set(POSIX_TIMER_LIB rt)


target_link_libraries(${TIMER_LIB} PUBLIC
							${POSIX_TIMER_LIB})