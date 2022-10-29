function(binFromSrcList)
	foreach(tutorial ${ARGV})
		set(fname "${tutorial}.cpp")
		add_executable(${tutorial} ${fname})
		target_include_directories(${tutorial} PRIVATE ${CMAKE_SOURCE_DIR}/src)
		target_link_libraries(${tutorial} PRIVATE assimp::assimp stelar3D)
		target_compile_options(${tutorial} PRIVATE
		                       #$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
		                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
		)
		set_target_properties(${tutorial} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
		if(IPOsupported)
    			set_property(TARGET ${tutorial} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
		endif()
	endforeach(tutorial)
endfunction()

add_subdirectory(00-getting_started)
add_subdirectory(01-lighting)
add_subdirectory(02-model-loading)
add_subdirectory(03-advanced)
add_subdirectory(04-advances-lighting)
