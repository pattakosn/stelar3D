#include "ogl_context.h"
#include "shader.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "element_array.h"
#include "texture.h"

int main(int, char*[])
{
	ogl_context ogl_context;

	Shader my_shader("03-00-texture_wood_container.vert", "03-00-texture_wood_container.frag");

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

    texture wood_box("wooden_container.jpg");

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    FlyCam camera;
    while (!ogl_context.should_close()) {
		glClear(GL_COLOR_BUFFER_BIT);
        // bind Texture
        wood_box.bind();
		my_shader.use();
		triangle.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		ogl_context.swap();
		ogl_context.check_keys(camera);
	}
	return EXIT_SUCCESS;
}