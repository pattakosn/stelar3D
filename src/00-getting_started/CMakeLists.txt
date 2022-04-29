set(chapters 00-getting_started 01-lighting)
set(00-getting_started
	00-00-setup_ogl4_glad
	01-00-triangle
	01-01-triangle-ebo
	01-02-triangles_two
	02-00-shaders
	02-01-shaders_uniform
	02-02-shaders_attributes
	02-03-shaders_attributes_shader_class
	03-00-texture_wood_container
	03-01-texture_wood_container_happy_face-2textures
	04-00-transformations
	04-01-transformations_exc
	05-00-coordinates
	05-01-coordinates_3dCube
	05-02-coordinates_10-3dCubes
	06-00-camera-autorotating
	06-01-camera-walk-around
	06-02-camera-walk-cam_class
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