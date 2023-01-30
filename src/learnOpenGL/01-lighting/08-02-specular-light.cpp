#include "shader.h"
#include "ogl_context.h"
#include "fly_cam.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "datapoints.h"

int main(int, char *[]) {
    ogl_context ogl_context;
    glEnable(GL_DEPTH_TEST);

    Shader itemShader("PN-MVP.vert", "obj_pos_color-specular.frag");
    Shader lampShader("PN-MVP.vert", "lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals, sizeof(cube_normals));

    attributes_binding_object item;
    item.bind();
    // position attribute
    item.add_attribute_floats_array(0, 3, 6, 0);
    // normal attribute
    item.add_attribute_floats_array(1, 3, 6, 3);

    attributes_binding_object lamp;
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    box.bind();
    // set the vertex attributes (only position data for our lamp)
    lamp.add_attribute_floats_array(0, 3, 6, 0);

    glm::vec3 lightPos(1.f, 1.0f, 1.0f);
    //glm::lookAt(glm::vec3(2.0f, 2.0f, 3.0f), lightPos, glm::vec3(0.0, 1.0f, 0.0f))
    FlyCam camera(glm::vec3(1.f, 1.f, 1.f));
    //camera.MovementSpeed = 100.;

    while (!ogl_context.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom),
                                      (float) ogl_context.screen_width() / (float) ogl_context.screen_height(), 0.1f,
                                      100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(0.8f);

        // don't forget to 'use' the corresponding shader program first (to set the uniform)
        itemShader.use();
        itemShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        itemShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        itemShader.setVec3("lightPos", lightPos);
        itemShader.setMat4("projection", projection);
        itemShader.setVec3("viewPos", camera.Position);
        itemShader.setMat4("view", view);
        itemShader.setMat4("model", model);

        // render the cube
        item.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        lamp.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        ogl_context.swap();
        ogl_context.check_keys(camera);
    }
    return EXIT_SUCCESS;
}