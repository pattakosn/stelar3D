#include "ogl_context.h"
#include "shader.h"
#include "datapoints.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "texture.h"

int main()
{
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

    texture floor("../assets/wood.png");
    Shader shader("../shaders/14-00-blinn-phong.vert", "../shaders/14-00-blinn-phong.frag");
    FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));
    glm::vec3 lightPos(0.0f, 1.0f, 0.0f);

    // render loop
    bool quit = false;
    bool blinn = false;
    planeCMDs.bind();
    while (!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        shader.use();
        shader.setInt("texture1", 0);
        glm::mat4 projection = glm::perspective(glm::radians(my_cam.Zoom), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", my_cam.GetViewMatrix());
        // set light uniforms
        shader.setVec3("viewPos", my_cam.Position);
        //lightPos[1] = sin(3 * ogl_app.time());
        shader.setVec3("lightPos", lightPos);
        shader.setInt("blinn", blinn);
        // floor
        floor.activate(0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

        ogl_app.swap();
        handle_events(quit, my_cam, ogl_app, blinn);
    }
    return EXIT_SUCCESS;
}
