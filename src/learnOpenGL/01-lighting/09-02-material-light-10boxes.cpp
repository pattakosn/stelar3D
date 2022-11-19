#include "shader.h"
#include "ogl_context.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "datapoints.h"

int main(int, char *[]) {
    ogl_context my_context;
    glEnable(GL_DEPTH_TEST);

    Shader itemShader("../shaders/PN-MVP.vert", "../shaders/PN-MatLight_Viewpos.frag");
    Shader lampShader("../shaders/PN-MVP.vert", "../shaders/lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals, sizeof(cube_normals));

    attributes_binding_object item;
    item.bind();
    // position attribute
    item.add_attribute_floats_array(0, 3, 6, 0);
    // normal attribute
    item.add_attribute_floats_array(1, 3, 6, 3);

    attributes_binding_object lamp;
    lamp.bind();
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    box.bind();
    // set the vertex attributes (only position data for our lamp)
    lamp.add_attribute_floats_array(0, 3, 6, 0);

    glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
    //glm::lookAt(glm::vec3(2.0f, 2.0f, 3.0f), lightPos, glm::vec3(0.0, 1.0f, 0.0f))
    FlyCam my_cam(glm::vec3(1.f, 1.f, 1.f));
    //my_cam.MovementSpeed = 100.;

    bool quit = false;
    while (!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(my_cam.Zoom),
                                      (float) my_context.screen_width() / (float) my_context.screen_height(),
                                      0.1f, 100.0f);
        glm::mat4 view = my_cam.GetViewMatrix();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 6; j++) {
                auto box = i * 6 + j;
                glm::mat4 model = glm::mat4(0.25f);
                model = glm::translate(model, cube_3x6_stacked_positions[box]);

                // don't forget to 'use' the corresponding shader program first (to set the uniform)
                itemShader.use();
                itemShader.setMat4("model", model);
                itemShader.setMat4("view", view);
                itemShader.setMat4("projection", projection);
                itemShader.setVec3("viewPos", my_cam.Position);
                itemShader.setVec3("material.ambient", cube_3x6_stacked_ambient[box]);
                itemShader.setVec3("material.diffuse", cube_3x6_stacked_diffuse[box]);
                itemShader.setVec3("material.specular", cube_3x6_stacked_specular[box]);
                itemShader.setFloat("material.shininess", cube_3x6_stacked_shininess[box]);
                itemShader.setVec3("light.position", lightPos);
                itemShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
                itemShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
                itemShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

                // render the cube
                item.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }


        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        lamp.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        my_context.swap();

        bool lol;
        handle_events(quit, my_cam, my_context, lol);
    }
    return EXIT_SUCCESS;
}