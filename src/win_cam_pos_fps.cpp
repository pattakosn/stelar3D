#include "win_cam_pos_fps.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "glm/gtc/type_ptr.hpp"

void win_cam_pos_fps_init(int x, int y) {
    ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_FirstUseEver);
}

void win_cam_pos_fps(FlyCam& my_cam) {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Stelar3D", NULL, ImGuiWindowFlags_NoBackground);
        ImGui::InputFloat3("camera position", glm::value_ptr(my_cam.Position));
        ImGui::InputFloat3("camera front", glm::value_ptr(my_cam.Front) );
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}