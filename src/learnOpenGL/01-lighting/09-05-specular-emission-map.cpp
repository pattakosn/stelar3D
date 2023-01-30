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

    Shader itemShader("PN-MVP-1T.vert", "PN-DiffusiveSpecularEmissionMatLight_Viewpos.frag");
    Shader lampShader("PN-MVP.vert", "lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals_textures, sizeof(cube_normals_textures));

    attributes_binding_object item;
    item.bind();
    // position attribute
    item.add_attribute_floats_array(0, 3, 8, 0);
    // normal attribute
    item.add_attribute_floats_array(1, 3, 8, 3);
    item.add_attribute_floats_array(2, 2, 8, 6);

    texture box_diffuse("container-diffusion.png");
    texture box_specular("container-specular.png");
    texture box_emission("container-emission.jpg");

    attributes_binding_object lamp;
    lamp.bind();
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    box.bind();
    // set the vertex attributes (only position data for our lamp)
    lamp.add_attribute_floats_array(0, 3, 8, 0);

    glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
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
        glm::mat4 model = glm::mat4(1.0f);

        // don't forget to 'use' the corresponding shader program first (to set the uniform)
        itemShader.use();
        itemShader.setMat4("model", model);
        itemShader.setMat4("view", view);
        itemShader.setMat4("projection", projection);
        itemShader.setVec3("viewPos", camera.Position);
        itemShader.setInt("material.diffuse", 0);
        itemShader.setInt("material.specular", 1);
        itemShader.setInt("material.emission", 2);
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
        box_diffuse.activate(GL_TEXTURE0);
        box_specular.activate(GL_TEXTURE1);
        box_emission.activate(GL_TEXTURE2);
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