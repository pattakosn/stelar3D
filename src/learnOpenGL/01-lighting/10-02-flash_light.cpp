#include "shader.h"
#include "ogl_context.h"
#include "fly_cam.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "datapoints.h"
#include "texture.h"

int main(int, char *[]) {
    ogl_context ogl_context;
    glEnable(GL_DEPTH_TEST);

    Shader itemShader("PN-MVP-1T.vert", "PN-DiffusiveSpecularMat-FlashLight_Viewpos.frag");
    Shader lampShader("PN-MVP.vert", "lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals_textures, sizeof(cube_normals_textures));

    attributes_binding_object item;
    item.bind();
    item.add_attribute_floats_array(0, 3, 8, 0);
    item.add_attribute_floats_array(1, 3, 8, 3);
    item.add_attribute_floats_array(2, 2, 8, 6);

    texture box_diffuse("container-diffusion.png");
    texture box_specular("container-specular.png");

    attributes_binding_object lamp;
    lamp.bind();
    box.bind();
    lamp.add_attribute_floats_array(0, 3, 8, 0);

    glm::vec3 lightPos(5.75f, 5.75f, 5.75f);
    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));
    //camera.MovementSpeed = 100.;

    while (!ogl_context.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to 'use' the corresponding shader program first (to set the uniform)
        itemShader.use();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.Zoom),
                                      (float) ogl_context.screen_width() / (float) ogl_context.screen_height(),
                                      0.1f,
                                      100.0f);
        itemShader.setMat4("projection", projection);

        glm::mat4 view = camera.GetViewMatrix();
        itemShader.setMat4("view", view);

        itemShader.setVec3("light.position", camera.Position);
        itemShader.setVec3("light.direction", camera.Front);
        itemShader.setFloat("light.cutOff",   glm::cos(glm::radians(12.5f)));
        itemShader.setVec3("viewPos", camera.Position);

        itemShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        itemShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        itemShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        itemShader.setInt("material.diffuse", 0);
        itemShader.setInt("material.specular", 1);
        itemShader.setFloat("material.shininess", 32.0f);

        itemShader.setFloat("light.constant",  1.0f);
        itemShader.setFloat("light.linear",    0.09f);
        itemShader.setFloat("light.quadratic", 0.032f);

        // render the cube
        item.bind();
        box_diffuse.activate(GL_TEXTURE0);
        box_specular.activate(GL_TEXTURE1);
        auto i = 0u;
        for( const auto& cube : cube_random_positions) {
            //if ( i == 10u )
            //    break;
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            auto angle = 20.0f * i++;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            itemShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        auto model = glm::mat4(1.0f);
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