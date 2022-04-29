#pragma once
#include "glad/glad.h"
#include <SDL2/SDL.h>
#include <string>
#include "ogl_debug.h"
#include <memory>

class ogl_context
{
public:
	ogl_context(const std::string win_title = std::string{"my OpenGL 4.5"},
	    const int w = 800,
	    const int h = 600,
	    const bool full_screen = false);
	~ogl_context();
	void info();
	void swap();
	float time();
	float dt() const { return dt_; };
	int screen_width() const { return width; };
	int screen_height() const { return height; };
private:
	int width;
	int height;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_GLContext gl_context;

    std::unique_ptr<ogl_debug> dbg;

	Uint32 tick_last = 0;
	Uint32 timeS = 0;
	float dt_ = 0.;

	void dt_update();
};
