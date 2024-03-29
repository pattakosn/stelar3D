#include "ogl_context.h"
#include "shader.h"
#include "datapoints.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"

int main(int, char *[])
{
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); // enabled by default on some drivers, but not all so always enable to make sure

    // build and compile shaders
    Shader shader("13-16-msaa.vert", "13-16-msaa.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    vertex_array cubeDAT;
    cubeDAT.add_buffer(cube_texture_coords,sizeof(cube_texture_coords));
    // setup cube VAO
    attributes_binding_object cubeCMD;
    cubeCMD.bind();
    cubeCMD.add_attribute_floats_array(0, 3, 5, 0);

    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));

    // render loop
    while (!ogl_app.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set transformation matrices		
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 1000.0f);
        shader.setMat4("projection", projection);
        shader.setMat4("view", camera.GetViewMatrix());
        shader.setMat4("model", glm::mat4(1.0f));

        cubeCMD.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        ogl_app.swap();
        ogl_app.check_keys(camera);
    }
    return EXIT_SUCCESS;
}