function(nadsad_Standard_Test NadsadTestName TestDir)

#EXE
add_executable(${NadsadTestName} ${TestDir}/main.cpp)

#DEFINITIONS
target_compile_definitions(${NadsadTestName} PRIVATE NADSAD_TEST_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

#OPTION
exe_emscripten_setup(${NadsadTestName} TRUE)

#OPTIONS
set_property(TARGET ${NadsadTestName} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${NadsadTestName} PROPERTY CXX_STANDARD_REQUIRED On)

if(NACMAKE_PROFILE)
	ENABLE_PROFILING(${NadsadTestName})
endif()

if(NACMAKE_WARNINGS)
	ENABLE_WARNINGS(${NadsadTestName} TRUE)
endif()

#LINK
target_link_libraries(${NadsadTestName} 
	PRIVATE 
	nadsad)

target_include_directories(${NadsadTestName}  
	PUBLIC 
	"$CACHE{NADSAD_INCLUDE_DIR}"
	$<BUILD_INTERFACE:$CACHE{NADSAD_INCLUDE_DIR}>)

#OUTPUT
SET_STANDARD_OUTPUT(${NadsadTestName})

endfunction()
