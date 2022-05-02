#include <iostream>
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
	ogl_context my_context;

	Shader my_shader("../shaders/04-00-2textures_moved.vert", "../shaders/04-00-2textures_moved.frag");

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

    texture wood_box("../assets/wooden_container.jpg", true);
    texture face("../assets/awesomeface.png", true);

    auto timeS = SDL_GetTicks();

my_shader.use();
glUniform1i(glGetUniformLocation(my_shader.ID, "texture0"), 0); // set it manually
my_shader.setInt("texture1", 1); // or with shader class


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	SDL_Event event;
	bool quit = false;
	while (!quit) {
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
auto timeN = SDL_GetTicks();
trans = glm::rotate(trans, glm::radians((float)((timeN-timeS)/30.f)), glm::vec3(0.0f, 0.0f, 1.0f));
//trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
my_shader.setMat4("transform", trans);
		triangle.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		my_context.swap();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					quit = true;
					break;
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					std::cout << "MESSAGE:Resizing window...\n";
					//resizeWindow(m_event.window.data1, m_event.window.data2);
					std::cout << "MESSAGE: Window [width x height] = " << event.window.data1 << " x " << event.window.data2 << "\n";//%d, %d\n", windowWidth, windowHeight);
					//m_camera->resizeWindow(windowWidth, windowHeight);
					//TODO: move this to gl_context so as to update WIDTH/HEIGHT
					glViewport(0, 0, event.window.data1, event.window.data2);
					break;
				}
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}