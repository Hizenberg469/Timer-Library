set(TIMERLIB_TEST_EXE timerlib_test)
set(WHEELTIMER_TEST_EXE wheeltimer_test)

set(TIMERLIB_TEST_SRC "timerlib_test.c")
set(WHEEL_TIMER_TEST_SRC "WheelTimerDemo.c")

target_compile_definitions(wheeltimer PRIVATE _POSIX_C_SOURCE=199309L)

add_executable(${TIMERLIB_TEST_EXE} 
					${TIMERLIB_TEST_SRC})

add_executable(${WHEELTIMER_TEST_EXE}
					${WHEEL_TIMER_TEST_SRC})

target_link_libraries(${TIMERLIB_TEST_EXE} PUBLIC
							${TIMER_LIB})

target_link_libraries(${WHEELTIMER_TEST_EXE} PUBLIC
								${WHEELTIMER_LIB})

target_include_directories(${TIMERLIB_TEST_EXE} PUBLIC
									${HEADER_DIR})

target_include_directories(${WHEELTIMER_TEST_EXE} PUBLIC
								${HEADER_DIR})