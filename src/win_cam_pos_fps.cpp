#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>
#include "win_cam_pos_fps.h"
#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glm/gtc/type_ptr.hpp"

std::string currentTime(std::chrono::time_point<std::chrono::system_clock> now)
{
    // you need to get milliseconds explicitly
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
    ) % 1000;
    // and that's a "normal" point of time with seconds
    auto timeNow = std::chrono::system_clock::to_time_t(now);

    std::ostringstream currentTimeStream;
    currentTimeStream << std::put_time(localtime(&timeNow), "%d.%m.%Y %H:%M:%S")
                      << "." << std::setfill('0') << std::setw(3) << milliseconds.count()
                      << " " << std::put_time(localtime(&timeNow), "%z");

    return currentTimeStream.str();
}

void win_cam_pos_fps_init(int x, int y) {
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
}

void win_cam_pos_fps(FlyCam& my_cam, ogl_context& my_context) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        int sdl_width = 0, sdl_height = 0, controls_width = 0;
        my_context.get_window_size(sdl_width, sdl_height);
        ImGui::SetNextWindowPos(ImVec2(sdl_width/2., sdl_height * 3. / 4.), ImGuiCond_Always);
        ImGui::SetNextWindowSize(
                ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)),
                ImGuiCond_Always
        );
        ImGui::Begin("Stelar3D", NULL, ImGuiWindowFlags_NoBackground);
        ImGui::InputFloat3("camera position", glm::value_ptr(my_cam.Position));
        ImGui::InputFloat3("camera front", glm::value_ptr(my_cam.Front) );
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //ImGui::Dummy(ImVec2(0.0f, 1.0f));
        //ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
        ImGui::Separator();
        ImGui::Text("%s", currentTime(std::chrono::system_clock::now()).c_str());
        //ImGui::Text("%s.", SDL_GetPlatform());
        ImGui::SameLine();
        ImGui::Text("Cores: %d", std::thread::hardware_concurrency());
        ImGui::SameLine();
        //ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}