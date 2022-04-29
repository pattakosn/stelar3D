#include <iostream>
#include "ogl_context.h"

int main(int, char* [])
{
	ogl_context my_ogl45_app;

	SDL_Event event;
	auto quit = false;
	while (!quit) {
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
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
