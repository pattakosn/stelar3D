#include "ogl_context.h"
#include "shader.h"
#include "datapoints.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"
#include "texture.h"

int main(int, char *[]) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("15-00-gamma.vert", "15-00-gamma.frag");
    shader.use();
    shader.setInt("floorTexture", 0);

    // plane VAO/VBO
    attributes_binding_object planeCMDs;
    planeCMDs.bind();
    vertex_array planeDAT;
    planeDAT.add_buffer(planePosNorTex, sizeof(planePosNorTex));
    planeCMDs.add_attribute_floats_array(0, 3, 8, 0);
    planeCMDs.add_attribute_floats_array(1, 3, 8, 3);
    planeCMDs.add_attribute_floats_array(2, 2, 8, 6);
    attributes_binding_object::unbind();

    texture floor("wood.png");
    texture floorGammaCorrected("wood.png", false, true);

    // lighting info
    glm::vec3 lightPositions[] = {
            glm::vec3(-3.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3 (1.0f, 0.0f, 0.0f),
            glm::vec3 (3.0f, 0.0f, 0.0f)
    };
    glm::vec3 lightColors[] = {
            glm::vec3(0.25),
            glm::vec3(0.50),
            glm::vec3(0.75),
            glm::vec3(1.00)
    };

    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));

    bool gammaEnabled = false;
    planeCMDs.bind();
    while (!ogl_app.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        glUniform3fv(glGetUniformLocation(shader.ID, "lightPositions"), 4, &lightPositions[0][0]);
        glUniform3fv(glGetUniformLocation(shader.ID, "lightColors"), 4, &lightColors[0][0]);
        shader.setVec3("viewPos", camera.Position);
        shader.setInt("gamma", gammaEnabled);
        // floor
        gammaEnabled ? floorGammaCorrected.activate(0) : floor.activate(0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if ( glfwGetKey(ogl_app.get_win(), GLFW_KEY_G) == GLFW_PRESS)
            gammaEnabled = !gammaEnabled;
        std::cout << (gammaEnabled ? "Gamma enabled" : "Gamma disabled") << std::endl;

        ogl_app.swap();
        ogl_app.check_keys(camera);
    }
    return EXIT_SUCCESS;
}