set(RT_SRC  "rt.c")
set(RT_MAIN	"rt_entry_expiration.c")

set(RT_EXE rtManager)

add_executable(${RT_EXE} ${RT_SRC}
						 ${RT_MAIN})

target_include_directories(${RT_EXE} PUBLIC
								${HEADER_DIR})

target_link_libraries(${RT_EXE} PUBLIC
							${TIMER_LIB})
