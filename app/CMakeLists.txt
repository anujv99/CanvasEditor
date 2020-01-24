add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)

file(GLOB_RECURSE application_source_files "src/*.*")

add_executable(Application ${application_source_files})

target_include_directories(Application PRIVATE "src")
target_include_directories(Application PRIVATE "vendor/glfw/include")
target_include_directories(Application PRIVATE "vendor/glad/include")

target_link_libraries(Application PRIVATE glfw)
target_link_libraries(Application PRIVATE glad)
target_link_libraries(Application PRIVATE opengl32)

option(LOGGING "Enable/Disable Logging" TRUE)
option(ASSERT "Enable/Disable Assers" TRUE)
option(UNICODE "Enable/Disable Unicode" FALSE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(__DEBUG__)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(__RELEASE__)
else()
	message(FATAL_ERROR "Undefined Configuration")
endif()

if (LOGGING)
	add_compile_definitions(__LOGGING__)
endif()

if (ASSERT)
	add_compile_definitions(__ASSERT__)
endif()

if (UNICODE)
	add_compile_definitions(__UNICODE__)
endif()

if (WIN32)
	add_compile_definitions(__WINDOWS__)
elseif(UNIX)
	add_compile_definitions(__UNIX__)
else()
	message(FATAL_ERROR "Only support windows and linux operating system")
endif()