#include <iostream>
#include "ogl_context.h"

int main(int, char* [])
{
	ogl_context ogl_context;

    while (!ogl_context.should_close()) {
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        ogl_context.swap();
	}

	return EXIT_SUCCESS;
}
