set(TIMERLIB_TEST_EXE timerlib_test)

set(TIMERLIB_TEST_SRC "timerlib_test.c")

add_executable(${TIMERLIB_TEST_EXE} 
					${TIMERLIB_TEST_SRC})

target_link_libraries(${TIMERLIB_TEST_EXE} PUBLIC
							${TIMER_LIB})

target_include_directories(${TIMERLIB_TEST_EXE} PUBLIC
									${HEADER_DIR})