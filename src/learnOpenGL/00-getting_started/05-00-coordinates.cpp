#include "shader.h"
#include "ogl_context.h"
#include "element_array.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "texture.h"

int main(int, char *[]) {
    ogl_context ogl_context;

    Shader my_shader("05-00-coordinates.vert", "05-00-coordinates.frag");

    float vertices[] = {
        // positions        // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    vertex_array triangle_data;
    triangle_data.add_buffer(vertices, sizeof(vertices));

    attributes_binding_object triangle;
    triangle.bind();
    // position attribute
    triangle.add_attribute_floats_array(0, 3, 5, 0);
    // texture coord attribute
    triangle.add_attribute_floats_array(1, 2, 5, 3);

    element_array triangle_idx;
    triangle_idx.add_buffer(indices, sizeof(indices));

    texture wood_box("wooden_container.jpg", true);
    texture face("awesomeface.png", true);

    my_shader.use();

    glUniform1i(glGetUniformLocation(my_shader.ID, "texture0"), 0); // set it manually
    my_shader.setInt("texture1", 1); // or with shader class
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    FlyCam camera;
    while (!ogl_context.should_close()) {
        glClear(GL_COLOR_BUFFER_BIT);
        my_shader.use();
        wood_box.activate(GL_TEXTURE0);
        face.activate(GL_TEXTURE1);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // note that we're translating the scene in the reverse direction of where we want to move
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(my_shader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(my_shader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        my_shader.setMat4("projection", projection);
        triangle.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        ogl_context.swap();
        ogl_context.check_keys(camera);
    }
    return EXIT_SUCCESS;
}