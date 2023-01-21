#pragma once
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "ogl_debug.h"
#include <string>
#include <memory>

class ogl_context
{
public:
    explicit ogl_context(const std::string win_title = std::string{"my OpenGL 4.5"},
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
    GLFWwindow* get_win() const { return window; };
    ImGuiIO* io;

private:
    int width;
    int height;

    GLFWwindow* window;

    std::unique_ptr<ogl_debug> dbg;

    Uint32 tick_last = 0;
    Uint32 timeS = 0;
    float dt_ = 0.;

    void dt_update();
};
