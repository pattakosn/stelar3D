set(chapters 01-lighting)
set(01-lighting
	07-00-light-scene
	08-00-ambient-light
	08-01-diffuse-light
	08-02-specular-light
	09-00-material
	09-01-material-light
	09-02-material-light-10boxes
	09-02-material-light-10boxes-LightColored
	09-03-diffuse-map
	09-04-specular-map
	09-05-specular-emission-map
	10-00-direct_light
	10-01-direct_spot_light
	10-02-flash_light
	10-03-flash_light_edges
	11-00-multiple_lights
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