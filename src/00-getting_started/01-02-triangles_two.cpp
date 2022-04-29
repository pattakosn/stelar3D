#include <iostream>
#include "ogl_context.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
};
const GLchar* v_shader_src = "#version 330 core \n\
                       layout (location = 0) in vec3 aPos;\n \
                       void main() { \n\
                           gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n \
                       }";
const GLchar* f_shader_src = "#version 330 core \n \
                       out vec4 FragColor; \n \
                       void main() { \n \
                           FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n \
                       } ";

int main(int, char*[])
{
	ogl_context my_ogl45_app;

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v_shader_src, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << " ERROR vertex shader compilation failed:\n\t" << infoLog << "\n";
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &f_shader_src, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << " ERROR fragment shader compilation failed:\n\t" << infoLog << "\n";
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << " ERROR linking shader program:\n\t" << infoLog << "\n";
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	vertex_array data;
    data.add_buffer(vertices, sizeof(vertices));

    attributes_binding_object triangle1;
    triangle1.bind();
    triangle1.add_attribute_floats_array(0, 3, 3, 0);
    triangle1.unbind();

    attributes_binding_object triangle2;
    triangle2.bind();
    triangle2.add_attribute_floats_array(0, 3, 3, 0);
    triangle2.unbind();

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	SDL_Event event;
	bool quit = false;
	while (!quit) {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		triangle1.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		triangle2.bind();
		glDrawArrays(GL_TRIANGLES, 1, 3);
		my_ogl45_app.swap();

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
					glViewport(0, 0, event.window.data1, event.window.data2);
					break;
				}
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}