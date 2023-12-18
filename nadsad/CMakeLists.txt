#LIBARY
add_library(nadsad)
set(NADSAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH INTERNAL)

#SOURCES
include(CMakeSources.cmake)

#OPTIONS
set_property(TARGET nadsad PROPERTY CXX_STANDARD 20)

if(NACMAKE_WARNINGS)
	ENABLE_WARNINGS(nadsad TRUE)
endif()

if(NACMAKE_PROFILING)
	ENABLE_PROFILING(nadsad)
endif()

#LINK
target_link_libraries(nadsad
	PUBLIC 
	natl)

target_include_directories(nadsad  
	PUBLIC 
	$<BUILD_INTERFACE:$CACHE{NATL_INCLUDE_DIR}>)

#OUTPUT
SET_STANDARD_OUTPUT(nadsad)