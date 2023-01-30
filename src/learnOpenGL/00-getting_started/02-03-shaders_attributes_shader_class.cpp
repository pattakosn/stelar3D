#include <iostream>
#include "ogl_context.h"
#include "shader.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"

float vertices[] = {
    //  x,     y,    z,    R,    G,    B
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // bottom right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
};

int main(int, char*[])
{
	ogl_context ogl_context;

	Shader my_shader("02-03-shaders_attributes_shader_class.vert", "02-03-shaders_attributes_shader_class.frag");

    vertex_array data;
    data.add_buffer(vertices, sizeof(vertices));
    attributes_binding_object triangle;
    triangle.bind();
    // position attribute
    triangle.add_attribute_floats_array(0, 3, 6, 0);
    // color attribute
    triangle.add_attribute_floats_array(1, 3, 6, 3);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    FlyCam camera;
    while (!ogl_context.should_close()) {
		glClear(GL_COLOR_BUFFER_BIT);
		my_shader.use();
		triangle.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
        ogl_context.swap();
        ogl_context.check_keys(camera);
	}
	return EXIT_SUCCESS;
}