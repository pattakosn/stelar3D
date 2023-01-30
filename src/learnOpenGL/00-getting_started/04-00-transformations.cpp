#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "ogl_context.h"
#include "element_array.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "texture.h"

int main(int,char*[])
{
	ogl_context ogl_context;

	Shader my_shader("04-00-2textures_moved.vert", "04-00-2textures_moved.frag");

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
//    // color attribute
//    triangle.add_attribute_floats_array(1, 3, 8, 3);
    // texture coord attribute
    triangle.add_attribute_floats_array(1, 2, 8, 6);

    element_array triangle_idx;
    triangle_idx.add_buffer(indices,sizeof(indices));

    texture wood_box("wooden_container.jpg", true);
    texture face("awesomeface.png", true);

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

glm::mat4 trans = glm::mat4(1.0f);
//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
//unsigned int transformLoc = glGetUniformLocation(my_shader.ID, "transform");
//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
trans = glm::rotate(trans, glm::radians((float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
//trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
my_shader.setMat4("transform", trans);
		triangle.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		ogl_context.swap();
        ogl_context.check_keys(camera);
	}
	return EXIT_SUCCESS;
}