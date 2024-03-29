#include "ogl_context.h"
#include "shader.h"
#include "texture.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "fly_cam.h"
#include "datapoints.h"

int main(int, char *[])
{
        ogl_context ogl_context;

        //Shader my_shader("../shaders/P-MVP-1T.vert", "../shaders/visualize-depthBuf.frag");
        Shader my_shader("P-MVP-1T.vert", "1T-apply.frag");

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // cube VAO
        attributes_binding_object cubeCMDs;
        cubeCMDs.bind();
        vertex_array cubeDAT;
        cubeDAT.add_buffer(cube_texture_coords, sizeof(cube_texture_coords));
        cubeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        cubeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        cubeCMDs.unbind();

        // plane VAO
        attributes_binding_object planeCMDs;
        planeCMDs.bind();
        vertex_array planeDAT;
        planeDAT.add_buffer(plane_vertices, sizeof(plane_vertices));
        planeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        planeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        planeCMDs.unbind();

        texture marbleTexture("marble.jpg");
        texture floorTexture("metal.png");

        // shader configuration
        my_shader.use();
        my_shader.setInt("texture1", 0);

        //glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
        FlyCam camera(glm::vec3(0.f, 1.f, 3.f));

        while (!ogl_context.should_close()) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                my_shader.use();
                glm::mat4 model = glm::mat4(1.0f);
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_context.screen_width() / (float)ogl_context.screen_height(), 0.1f, 100.0f);
                my_shader.setMat4("view", view);
                my_shader.setMat4("projection", projection);
                // cubes
                cubeCMDs.bind();
                marbleTexture.activate(GL_TEXTURE0); // glActiveTexture(GL_TEXTURE0);
                //glBindTexture(GL_TEXTURE_2D, cubeTexture);
                model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
                my_shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
                my_shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                // floor
                planeCMDs.bind();
                floorTexture.activate(GL_TEXTURE0);
                my_shader.setMat4("model", glm::mat4(1.0f));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                ogl_context.swap();
                ogl_context.check_keys(camera);
        }
        return EXIT_SUCCESS;
}
