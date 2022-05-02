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
	ogl_context my_context;

	Shader my_shader("../shaders/02-03-shaders_attributes_shader_class.vert", "../shaders/02-03-shaders_attributes_shader_class.frag");

    vertex_array data;
    data.add_buffer(vertices, sizeof(vertices));
    attributes_binding_object triangle;
    triangle.bind();
    // position attribute
    triangle.add_attribute_floats_array(0, 3, 6, 0);
    // color attribute
    triangle.add_attribute_floats_array(1, 3, 6, 3);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		glClear(GL_COLOR_BUFFER_BIT);
		my_shader.use();
		triangle.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
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