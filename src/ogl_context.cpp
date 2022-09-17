#include <iostream>
#include "ogl_context.h"

static void sdl_die(const char * message)
{
	std::cerr << message << " : " << std::string(SDL_GetError()) << "\n";
	exit(EXIT_FAILURE);
}

void checkSDLError(int
#ifndef NDEBUG
		line = -1
#endif
    )

{
#ifndef NDEBUG
    if (const char *error = SDL_GetError(); *error != '\0')
	{
		std::cerr << "SDL: " << std::string(error);
		if (line != -1)
			std::cerr << " at line: " << line ;
		std::cerr << "\n";
		SDL_ClearError();
	}
#endif
}

ogl_context::~ogl_context()
{
    dbg.reset();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(gl_context);
//    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // this is called by atexit(): SDL_Quit();
}

ogl_context::ogl_context(const std::string win_title, const int w, const int h, const bool full_screen) : width{w}, height{h}
{
	//SDL_SetMainReady();
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) //SDL_INIT_EVERYTHING
		sdl_die("SDL: init failed");
	atexit (SDL_Quit);

	// I do not know what this does || if it is required but it seems to not harm anything
	if (SDL_GL_LoadLibrary(nullptr)!=0 ) {
		checkSDLError(__LINE__);
		sdl_die("SDL: GL Load Library failed");
	}

	// Request an OpenGL 4.5 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	//Request a stencil buffer of at least 1bit per pixel
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 1 );

	// Create the window
	if (full_screen)
		window = SDL_CreateWindow(win_title.c_str(),
		                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		                          0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	else
		window = SDL_CreateWindow(win_title.c_str(),
		                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		                          width, height,
		                          SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == nullptr) {
		checkSDLError(__LINE__);
		sdl_die("SDL: CreateWindow failed");
	}

//	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//	if (renderer == nullptr) {
//        checkSDLError(__LINE__);
//		sdl_die("SDL2: renderer created failed");
//    }

	gl_context = SDL_GL_CreateContext(window);
	if (gl_context == nullptr) {
		checkSDLError(__LINE__);
	    sdl_die("SDL: OpenGL context creation failed");
	}
	SDL_GL_MakeCurrent(window, gl_context);
	// Enable vsync
	SDL_GL_SetSwapInterval(1);

	// Load GL extensions using glad
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cerr << "GLAD: Failed to initialize OpenGL context." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	(void)*io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(NULL);

	// Disable depth test and face culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	int tmp_w, tmp_h;
	SDL_GetWindowSize(window, &tmp_w, &tmp_h);
	std::cout << "[ app  ] viewport size: " << tmp_w << "x" << tmp_h << "\n";
	glViewport(0, 0, tmp_w, tmp_h);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);

	SDL_WarpMouseInWindow(window, width / 2, height / 2);

	timeS = SDL_GetTicks();
	info();

	dbg = std::make_unique<ogl_debug>();
}

void ogl_context::info( )
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	std::cout << "[ app  ] OpenGL loaded\n";
	// Loaded OpenGL successfully.
	std::cout << "[OpenGL] OpenGL version loaded: " << GLVersion.major << "." << GLVersion.minor << std::endl;
	std::cout << "[OpenGL] Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "[OpenGL] Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "[OpenGL] Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "[OpenGL] Version GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	std::cout << "[OpenGL] Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
}

void ogl_context::swap( )
{
	SDL_GL_SwapWindow(window);
	dt_update();
}

void ogl_context::dt_update( )
{
	auto tick_cur = SDL_GetTicks();
	dt_ = (tick_cur - tick_last)/1000.f;
	tick_last = tick_cur;
}

float ogl_context::time( )
{
	return (SDL_GetTicks() - timeS)/1000.f;
}
