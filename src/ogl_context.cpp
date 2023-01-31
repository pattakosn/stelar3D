#include "ogl_context.h"
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//}

static void glfw_error_callback(int /*error*/, const char* description) {
    fprintf(stderr, "[GLFW] Error: %s\n", description);
}

ogl_context::~ogl_context() {
    dbg.reset();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

ogl_context::ogl_context(const std::string& win_title, const int w, const int h, const bool full_screen) : width{w}, height{h} {
    glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
        throw std::runtime_error("Error initializing GLFW");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, true);
    glfwWindowHint(GLFW_SAMPLES, 4 );
    // Create the window
    if (full_screen)
        window = glfwCreateWindow(w, h, win_title.c_str(), glfwGetPrimaryMonitor(), nullptr);
    else
        window = glfwCreateWindow(w, h, win_title.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Error creating GLFW window");
    }

    glfwMakeContextCurrent(window);
    //glfwSetKeyCallback(window, key_callback);

	// Enable vsync
    glfwSwapInterval(1);

	// Load GL extensions using glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        glfwTerminate();
        throw std::runtime_error("Error loading GLAD2 OpenGL extension library");
    } else {
        std::cout << "GLAD2 OpenGL extension loaded:" << GLAD_VERSION_MAJOR(version) << "."
                  << GLAD_VERSION_MINOR(version) << std::endl;
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
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr);

	// Disable depth test and face culling.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    //glEnable(GL_FRAMEBUFFER_SRGB);

	int tmp_w, tmp_h;
    glfwGetFramebufferSize(window, &tmp_w, &tmp_h);
	std::cout << "[ app  ] viewport size: " << tmp_w << "x" << tmp_h << "\n";
	glViewport(0, 0, tmp_w, tmp_h);
	glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    timeS = glfwGetTime();
	info();

	dbg = std::make_unique<ogl_debug>();
}

void ogl_context::info( ) {
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

    std::cout << "[ app  ] OpenGL loaded\n";
    // Loaded OpenGL successfully.
    std::cout << "[OpenGL] Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "[OpenGL] Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "[OpenGL] Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "[OpenGL] Version GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "[OpenGL] Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
}

void ogl_context::swap( ) {
    glfwSwapBuffers(window);
    glfwPollEvents();
    dt_update();
}

void ogl_context::dt_update( ) {
	auto tick_cur    = glfwGetTime();
	dt_ = (tick_cur - tick_last)/1000.f;
	tick_last = tick_cur;
}

float ogl_context::time( ) {
	return (glfwGetTime() - timeS)/1000.f;
}

bool ogl_context::should_close() {
    return glfwWindowShouldClose(window);
}

void ogl_context::get_window_size(int &width, int &height) {
    glfwGetFramebufferSize(window, &width, &height);
}

void ogl_context::check_keys(FlyCam &my_cam) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (   glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS
           || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        my_cam.ProcessKeyboard(FORWARD, dt_);
    if (   glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS
           || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        my_cam.ProcessKeyboard(BACKWARD, dt_);
    if (   glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS
           || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        my_cam.ProcessKeyboard(LEFT, dt_);
    if (   glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS
           || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        my_cam.ProcessKeyboard(RIGHT, dt_);
    if (   glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        my_cam.ProcessKeyboard(UP, dt_);
    if (   glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        my_cam.ProcessKeyboard(DOWN, dt_);

    static bool wireframe = false;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wireframe = false;
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            wireframe = true;
        }
    }
    static bool depthTest = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (depthTest) {
            glDepthFunc(GL_LESS);
            depthTest = false;
        } else {
            glDepthFunc(GL_ALWAYS);
            depthTest = true;
        }
    }
    if (   glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        my_cam.reset();

//			case SDL_MOUSEWHEEL:
//				switch (event.wheel.type) {
//					case SDL_MOUSEWHEEL:
//						my_cam.ProcessMouseScroll((float) event.wheel.y);
//						break;
//
//					default:
//						break;
//				}
//				break;
//			case SDL_MOUSEMOTION:
//				my_cam.ProcessMouseMovement((float) event.motion.xrel, (float) event.motion.yrel);
//				break;
//
//		}
//	}
}