#include "ogl_context.h"
#include "shader.h"
#include "element_array.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "texture.h"
#include "stb_image.h"

int main(int, char*[])
{
	ogl_context ogl_context;

	Shader my_shader("03-01-texture_wood_container_happy_face.vert", "03-01-texture_wood_container_happy_face.frag");

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
    vertex_array triangle_data;
    triangle_data.add_buffer(vertices, sizeof(vertices));

    attributes_binding_object triangle;
    triangle.bind();
    // position attribute
    triangle.add_attribute_floats_array(0, 3, 8, 0);
    // color attribute
    triangle.add_attribute_floats_array(1, 3, 8, 3);
    // texture coord attribute
    triangle.add_attribute_floats_array(2, 2, 8, 6);

    element_array triangle_idx;
    triangle_idx.add_buffer(indices,sizeof(indices));

    texture wood_box("wooden_container.jpg", true);
    texture face( "awesomeface.png", true);

my_shader.use();
glUniform1i(glGetUniformLocation(my_shader.ID, "texture0"), 0); // set it manually
my_shader.setInt("texture1", 1); // or with shader class
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    FlyCam camera;
    while (!ogl_context.should_close()) {
		glClear(GL_COLOR_BUFFER_BIT);
		my_shader.use();
wood_box.activate(GL_TEXTURE0);
face.activate(GL_TEXTURE1);

		triangle.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		ogl_context.swap();
        ogl_context.check_keys(camera);
	}
	return EXIT_SUCCESS;
}