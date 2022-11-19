#include "shader.h"
#include "ogl_context.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "datapoints.h"

int main(int, char *[]) {
    ogl_context my_context;
    glEnable(GL_DEPTH_TEST);

    Shader itemShader("../shaders/P-MVP.vert", "../shaders/obj_pos_color.frag");
    Shader lampShader("../shaders/P-MVP.vert", "../shaders/lamp.frag");

    vertex_array box;
    box.add_buffer(cube_normals, sizeof(cube_normals));

    attributes_binding_object item;
    item.bind();
    // only use position attribute
    item.add_attribute_floats_array(0, 3, 6, 0);

    attributes_binding_object lamp;
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    box.bind();
    // set the vertex attributes (only position data for our lamp)
    lamp.add_attribute_floats_array(0, 3, 3, 0);

    FlyCam my_cam(glm::vec3(0.0f, 0.0f, 3.0f));
    //my_cam.MovementSpeed = 100.;
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    bool quit = false;
    while (!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(my_cam.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = my_cam.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // don't forget to 'use' the corresponding shader program first (to set the uniform)
        itemShader.use();
        itemShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        itemShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        itemShader.setMat4("projection", projection);
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
        my_context.swap();

        bool lol;
        handle_events(quit, my_cam, my_context, lol);
    }
    return EXIT_SUCCESS;
}