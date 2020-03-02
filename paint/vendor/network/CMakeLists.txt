
file(GLOB_RECURSE network_source_files "src/*.*")

add_library(Network ${network_source_files})

target_include_directories(Network PRIVATE "src")

option(LOGGING "Enable/Disable Logging" TRUE)
option(ASSERT "Enable/Disable Assers" TRUE)

if (LOGGING)
	add_compile_definitions(__LOGGING__)
endif()

if (ASSERT)
	add_compile_definitions(__ASSERT__)
endif()

if (WIN32)
	add_compile_definitions(__WINDOWS__)
	target_link_libraries(Network PRIVATE Ws2_32)
elseif(UNIX)
	add_compile_definitions(__UNIX__)
else()
	message(FATAL_ERROR "Only support windows and linux operating system")
endif()
