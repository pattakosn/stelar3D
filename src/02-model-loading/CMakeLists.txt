set(chapters 02-model-loading)
set(02-model-loading
	12-00-backpack-model
)

foreach(chapter ${chapters})
	foreach(tutorial ${${chapter}})
		set(fname "${tutorial}.cpp")
		add_executable(${tutorial} ${fname})
		target_include_directories(${tutorial} PRIVATE ${CMAKE_SOURCE_DIR}/src)
		target_link_libraries(${tutorial} PRIVATE ${LIBS} stelar3D)
		target_compile_options(${tutorial} PRIVATE
		                       #$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
		                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
		)
		set_target_properties(${tutorial} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
	endforeach(tutorial)
endforeach(chapter)