#include "ogl_context.h"
#include "shader.h"
#include "texture.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "fly_cam.h"
#include "datapoints.h"
#include <map>

int main(int, char *[])
{
        ogl_context ogl_context;
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Shader shader("P-MVP-1T.vert", "1T-apply.frag");

        // cubeVAO, cubeVBO
        attributes_binding_object cubeCMDs;
        cubeCMDs.bind();
        vertex_array cubeDAT;
        cubeDAT.add_buffer(cube_texture_coords, sizeof(cube_texture_coords));
        cubeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        cubeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        cubeCMDs.unbind();
        // planeVAO, planeVBO
        attributes_binding_object planeCMDs;
        planeCMDs.bind();
        vertex_array planeDAT;
        planeDAT.add_buffer(plane_vertices, sizeof(plane_vertices));
        planeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        planeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        planeCMDs.unbind();
        // transparentVAO, transparentVBO;
        attributes_binding_object transparentCMDs;
        transparentCMDs.bind();
        vertex_array transparentDAT;
        transparentDAT.add_buffer(transparent_vertices, sizeof(transparent_vertices));
        transparentCMDs.add_attribute_floats_array(0, 3, 5, 0);
        transparentCMDs.add_attribute_floats_array(1, 2, 5, 3);
        transparentCMDs.unbind();

        texture marbleTexture("marble.jpg");
        texture floorTexture("metal.png");
        texture window("transparent_window.png");
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        std::vector<glm::vec3> windows;
        windows.emplace_back(-1.5f, 0.0f, -0.48f);
        windows.emplace_back(1.5f, 0.0f, 0.51f);
        windows.emplace_back(0.0f, 0.0f, 0.7f);
        windows.emplace_back(-0.3f, 0.0f, -2.3f);
        windows.emplace_back(0.5f, 0.0f, -0.6f);

        // shader configuration
        shader.use();
        shader.setInt("texture1", 0);

        FlyCam camera(glm::vec3(0.f, 1.f, 3.f));

        while (!ogl_context.should_close()) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // sort the transparent windows before rendering
                std::map<float, glm::vec3> sorted;
                for (auto & win : windows) {
                        float distance = glm::length(camera.Position - win);
                        sorted[distance] = win;
                }

                shader.use();
                glm::mat4 model = glm::mat4(1.0f);
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_context.screen_width() / (float)ogl_context.screen_height(), 0.1f, 100.0f);
                shader.setMat4("view", view);
                shader.setMat4("projection", projection);

                // cubes
                cubeCMDs.bind();
                marbleTexture.activate(GL_TEXTURE0);
                model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                // floor
                planeCMDs.bind();
                floorTexture.activate(GL_TEXTURE0);
                shader.setMat4("model", glm::mat4(1.0f));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // windows (from furthest to nearest)
                transparentCMDs.bind();
                window.activate(GL_TEXTURE0);
                for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
                        model = glm::mat4(1.0f);
                        model = glm::translate(model, it->second);
                        shader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 6);
                }

                ogl_context.swap();
            ogl_context.check_keys(camera);
        }
        return EXIT_SUCCESS;
}
