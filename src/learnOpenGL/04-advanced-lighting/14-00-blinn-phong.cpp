#include "ogl_context.h"
#include "shader.h"
#include "datapoints.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"
#include "texture.h"

int main(int, char *[] ) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    Shader shader("14-00-blinn-phong.vert", "14-00-blinn-phong.frag");
    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));
    glm::vec3 lightPos(0.0f, 1.0f, 0.0f);

    // render loop
    bool blinn = false;
    planeCMDs.bind();
    while (!ogl_app.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        shader.use();
        shader.setInt("texture1", 0);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.GetViewMatrix());
        // set light uniforms
        shader.setVec3("viewPos", camera.Position);
        //lightPos[1] = sin(3 * ogl_app.time());
        shader.setVec3("lightPos", lightPos);
        shader.setInt("blinn", blinn);
        // floor
        floor.activate(0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if ( glfwGetKey(ogl_app.get_win(), GLFW_KEY_B) == GLFW_PRESS)
            blinn = !blinn;
        std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

        ogl_app.swap();
        ogl_app.check_keys(camera);
    }
    return EXIT_SUCCESS;
}
