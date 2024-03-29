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

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        Shader my_shader("P-MVP-1T.vert", "1T-apply.frag");
        Shader my_shader_outline("P-MVP-1T.vert", "greenOutline.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // cube VAO
        attributes_binding_object cubeCMDs;
        cubeCMDs.bind();
        vertex_array cubeDAT;
        cubeDAT.add_buffer(cube_texture_coords, sizeof(cube_texture_coords));
        cubeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        cubeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        attributes_binding_object::unbind();

        // plane VAO
        attributes_binding_object planeCMDs;
        planeCMDs.bind();
        vertex_array planeDAT;
        planeDAT.add_buffer(plane_vertices, sizeof(plane_vertices));
        planeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        planeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        attributes_binding_object::unbind();

        texture marbleTexture("marble.jpg");
        texture floorTexture("metal.png");

        // shader configuration
        my_shader.use();
        my_shader.setInt("texture1", 0);

        FlyCam camera(glm::vec3(0.f, 0.f, 3.f));

        while (!ogl_context.should_close()) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                my_shader_outline.use();
                glm::mat4 model = glm::mat4(1.0f);
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_context.screen_width() / (float)ogl_context.screen_height(), 0.1f, 100.0f);
                my_shader_outline.setMat4("view", view);
                my_shader_outline.setMat4("projection", projection);

                my_shader.use();
                my_shader.setMat4("view", view);
                my_shader.setMat4("projection", projection);

                // floor, make sure we don't update the stencil buffer while drawing the floor
                glStencilMask(0x00);
                planeCMDs.bind();
                floorTexture.activate(GL_TEXTURE0);
                my_shader.setMat4("model", glm::mat4(1.0f));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);

                // 1st. render pass, draw objects as normal, writing to the stencil buffer
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
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

                // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
                // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
                // the objects' size differences, making it look like borders.
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);
                my_shader_outline.use();
                float scale = 1.1f;
                cubeCMDs.bind();
                marbleTexture.activate(GL_TEXTURE0);
                model = glm::mat4(1.f);
                model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
                model = glm::scale(model, glm::vec3(scale, scale, scale));
                my_shader_outline.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(scale, scale, scale));
                my_shader_outline.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 0, 0xFF);
                glEnable(GL_DEPTH_TEST);

                ogl_context.swap();
            ogl_context.check_keys(camera);
        }
        return EXIT_SUCCESS;
}
