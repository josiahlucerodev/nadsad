#VERSION
cmake_minimum_required (VERSION 3.9)

#PROJECT 
project(nadsad)

#DEPENDENCIES
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR) #IS ROOT
	#NACTEMPLATE
	include(externals/nactemplate/externals_nactemplate.cmake)

	#NATL
	add_subdirectory(externals/natl)
else()
	#NACTEMPLATE
	if(NOT TARGET nactemplate)
		message(FATAL_ERROR "nadsad: requires nactemplate")
	endif()

	#NATL
	if(NOT TARGET natl) 
		message(FATAL_ERROR "nadsad: requires natl")
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