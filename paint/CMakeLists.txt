add_subdirectory(vendor/network)

file(GLOB_RECURSE paint_source_files "src/*.*")

add_executable(Paint ${paint_source_files})

target_include_directories(Paint PRIVATE "src")
target_include_directories(Paint PRIVATE "vendor/network/src")
target_include_directories(Paint PRIVATE "../app/src")
target_include_directories(Paint PUBLIC "../app/vendor/prevmath/include")
target_include_directories(Paint PUBLIC "../app/vendor/lua")

target_link_libraries(Paint PRIVATE Application)
target_link_libraries(Paint PRIVATE Network)

option(LOGGING "Enable/Disable Logging" TRUE)
option(ASSERT "Enable/Disable Assertions" TRUE)
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