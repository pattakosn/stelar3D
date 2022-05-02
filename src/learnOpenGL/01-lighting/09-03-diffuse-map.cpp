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

    Shader itemShader("../shaders/PN-MVP-1T.vert", "../shaders/PN-DiffusiveMatLight_Viewpos.frag");
    Shader lampShader("../shaders/PN-MVP.vert", "../shaders/lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals_textures, sizeof(cube_normals_textures));

    attributes_binding_object item;
    item.bind();
    // position attribute
    item.add_attribute_floats_array(0, 3, 8, 0);
    // normal attribute
    item.add_attribute_floats_array(1, 3, 8, 3);
    item.add_attribute_floats_array(2, 2, 8, 6);

    texture wood_diffuse_box("../assets/container-diffusion.png");

    attributes_binding_object lamp;
    lamp.bind();
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    box.bind();
    // set the vertex attributes (only position data for our lamp)
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
        itemShader.setMat4("model", model);
        itemShader.setMat4("view", view);
        itemShader.setMat4("projection", projection);
        itemShader.setVec3("viewPos", my_cam.Position);
        itemShader.setInt("material.diffuse", 0);
        itemShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        itemShader.setFloat("material.shininess", 16.0f);
        itemShader.setVec3("light.position", lightPos);
        glm::vec3 lightColor(1.f, 1.f, 1.f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        itemShader.setVec3("light.ambient", ambientColor);
        itemShader.setVec3("light.diffuse", diffuseColor);
        //itemShader.setVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        //itemShader.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        itemShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // render the cube
        item.bind();
        wood_diffuse_box.activate(GL_TEXTURE0);
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
        my_context.swap();

        handle_events(quit, my_cam, my_context);
    }
    return EXIT_SUCCESS;
}