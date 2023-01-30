#pragma once
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ogl_debug.h"
#include "fly_cam.h"
#include <string>
#include <memory>

class ogl_context
{
public:
    explicit ogl_context(const std::string& win_title = std::string{"stelar 3D"},
                         int w = 800,
                         int h = 600,
                         bool full_screen = false);
    ~ogl_context();
    void info();
    void swap();
    float time();
    double dt() const { return dt_; };
    int screen_width() const { return width; };
    int screen_height() const { return height; };
    GLFWwindow* get_win() const { return window; };
    ImGuiIO* io;
    bool should_close();
    void get_window_size(int& width, int& height);
    void check_keys(FlyCam &my_cam);
    const GLFWwindow* GetGlfwWindow() const { return window; };

private:
    int width;
    int height;

    GLFWwindow* window;

    std::unique_ptr<ogl_debug> dbg;

    double tick_last = 0.;
    double timeS = 0.;
    double dt_ = 0.;

    void dt_update();
};
