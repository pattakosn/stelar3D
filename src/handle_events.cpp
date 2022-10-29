#include "handle_events.h"
#include <iostream>

void handle_events(bool &quit, FlyCam &my_cam, ogl_context &my_context, bool &check_b) {
	SDL_Event event;
	static bool wireframe = false;
	static bool depthTest = false;
	while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
					case SDLK_q:
						quit = true;
						break;
					case SDLK_w:
					case SDLK_UP:
						my_cam.ProcessKeyboard(FORWARD, my_context.dt());
						break;
					case SDLK_s:
					case SDLK_DOWN:
						my_cam.ProcessKeyboard(BACKWARD, my_context.dt());
						break;
					case SDLK_a:
					case SDLK_LEFT:
						my_cam.ProcessKeyboard(LEFT, my_context.dt());
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						my_cam.ProcessKeyboard(RIGHT, my_context.dt());
						break;
					case SDLK_r:
						my_cam.reset();
						break;
					case SDLK_f:
						my_cam.ProcessKeyboard(UP, my_context.dt());
						break;
					case SDLK_v:
						my_cam.ProcessKeyboard(DOWN, my_context.dt());
						break;
                    case SDLK_b:
                        check_b = !check_b;
                        break;
					case SDLK_t:
						if ( wireframe ) {
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
							wireframe = false;
						} else {
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							wireframe  = true;
						}
						break;
					case SDLK_p:
						if ( depthTest ) {
							glDepthFunc(GL_LESS);
							depthTest = false;
						} else {
							glDepthFunc(GL_ALWAYS);
							depthTest = true;
						}
						break;
				}
				break;
			case SDL_MOUSEWHEEL:
				switch (event.wheel.type) {
					case SDL_MOUSEWHEEL:
						my_cam.ProcessMouseScroll((float) event.wheel.y);
						break;

					default:
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				my_cam.ProcessMouseMovement((float) event.motion.xrel, (float) event.motion.yrel);
				break;
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						std::cout << "MESSAGE:Resizing window...\n";
						//resizeWindow(m_event.window.data1, m_event.window.data2);
						std::cout << "MESSAGE: Window [width x height] = " << event.window.data1 << " x "
							<< event.window.data2 << "\n";//%d, %d\n", windowWidth, windowHeight);
						//m_camera->resizeWindow(windowWidth, windowHeight);
						//TODO: move this to gl_context so as to update WIDTH/HEIGHT
						glViewport(0, 0, event.window.data1, event.window.data2);
						break;
				}
				break;
		}
	}
}
