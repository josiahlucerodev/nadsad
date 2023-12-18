#VERSION
cmake_minimum_required (VERSION 3.9)

#PROJECT 
project(nadsad)

#DEPENDENCIES
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR) #IS ROOT
	#NACMAKE
	include(externals/nacmake/externals_nacmake.cmake)
else()
	#NACMAKE
	if(NOT TARGET nacmake)
		message(FATAL_ERROR "nadsad: requires nacmake")
	endif()
endif()

#IS ROOT
CHECK_IF_IS_ROOT(NADSAD_IS_ROOT)

#PROJECT LIBRARY
add_subdirectory(nadsad)

#TEST
if(NADSAD_IS_ROOT)
	message("nadsad: tests")
	add_subdirectory(tests)
endif()