#include <iostream>
#include "ogl_context.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
const GLchar *v_shader_src = "#version 330 core \n\
                       layout (location = 0) in vec3 aPos;\n \
		               out vec4 vertexColor; // specify a color output to the fragment shader\n \
                       void main() { \n\
                           gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n \
                           vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color \n \
                       }";
const GLchar *f_shader_src = "#version 330 core \n \
                       out vec4 FragColor; \n \
                       in vec4 vertexColor; \n \
                       void main() { \n \
                           FragColor = vertexColor; \n \
                       } ";

int main(int, char*[])
{
	ogl_context ogl_context;

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &v_shader_src, nullptr);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << " ERROR vertex shader compilation failed:\n\t" << infoLog << "\n";
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &f_shader_src, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << " ERROR fragment shader compilation failed:\n\t" << infoLog << "\n";
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << " ERROR linking shader program:\n\t" << infoLog << "\n";
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    vertex_array data;
    data.add_buffer(vertices, sizeof(vertices));
    attributes_binding_object triangle;
    triangle.bind();
    triangle.add_attribute_floats_array(0, 3, 3, 0);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    FlyCam camera;
	while (!ogl_context.should_close()) {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
        triangle.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
        ogl_context.swap();
        ogl_context.check_keys(camera);
	}
    return EXIT_SUCCESS;
}