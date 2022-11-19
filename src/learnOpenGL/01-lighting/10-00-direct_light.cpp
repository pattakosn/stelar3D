#include "shader.h"
#include "ogl_context.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "datapoints.h"
#include "texture.h"

int main(int, char *[]) {
    ogl_context my_context;
    glEnable(GL_DEPTH_TEST);

    Shader itemShader("../shaders/PN-MVP-1T.vert", "../shaders/PN-DiffusiveSpecularMat-Directional_Viewpos.frag");
    Shader lampShader("../shaders/PN-MVP.vert", "../shaders/lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals_textures, sizeof(cube_normals_textures));

    attributes_binding_object item;
    item.bind();
    item.add_attribute_floats_array(0, 3, 8, 0);
    item.add_attribute_floats_array(1, 3, 8, 3);
    item.add_attribute_floats_array(2, 2, 8, 6);

    texture box_diffuse("../assets/container-diffusion.png");
    texture box_specular("../assets/container-specular.png");

    attributes_binding_object lamp;
    lamp.bind();
    box.bind();
    lamp.add_attribute_floats_array(0, 3, 8, 0);

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
                                      (float) my_context.screen_width() / (float) my_context.screen_height(), 0.1f,
                                      100.0f);
        glm::mat4 view = my_cam.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // don't forget to 'use' the corresponding shader program first (to set the uniform)
        itemShader.use();
        itemShader.setMat4("view", view);
        itemShader.setMat4("projection", projection);
        itemShader.setVec3("viewPos", my_cam.Position);
        itemShader.setInt("material.diffuse", 0);
        itemShader.setInt("material.specular", 1);
        itemShader.setFloat("material.shininess", 16.0f);
        itemShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        glm::vec3 lightColor(1.f, 1.f, 1.f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        itemShader.setVec3("light.ambient", ambientColor);
        itemShader.setVec3("light.diffuse", diffuseColor);
        itemShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // render the cube
        item.bind();
        box_diffuse.activate(GL_TEXTURE0);
        box_specular.activate(GL_TEXTURE1);
        auto i = 0u;
        for( const auto& cube : cube_random_positions) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            auto angle = 20.0f * i++;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            itemShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
        my_context.swap();

        bool lol;
        handle_events(quit, my_cam, my_context, lol);
    }
    return EXIT_SUCCESS;
}