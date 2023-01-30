#include "ogl_context.h"
#include "fly_cam.h"
#include "shader.h"
#include "datapoints.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "texture.h"
#include "frame_buffer.h"

int main(int, char*[])
{
        ogl_context ogl_context;//"sth", 480, 320);
        glEnable(GL_DEPTH_TEST);

        // build and compile shaders
        Shader shader("P-MVP-1T.vert", "1T-apply.frag");
        //Shader screenShader("P-1T.vert", "1T-apply_inversed.frag");
        //Shader screenShader("P-1T.vert", "1T-apply_greyscaled.frag");
        Shader screenShader("P-1T.vert", "1T-apply_greyscaledWeighted.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // cube VAO
        attributes_binding_object cubeCMDs;
        cubeCMDs.bind();
        vertex_array cubeDAT;
        cubeDAT.add_buffer(cube_texture_coords, sizeof(cube_texture_coords));
        cubeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        cubeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        // plane VAO
        attributes_binding_object planeCMDs;
        planeCMDs.bind();
        vertex_array planeDAT;
        planeDAT.add_buffer(plane_vertices, sizeof(plane_vertices));
        planeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        planeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        // screen quad VAO
        attributes_binding_object mirror_quadCMDs;
        mirror_quadCMDs.bind();
        vertex_array mirrorQuadDAT;
        mirrorQuadDAT.add_buffer(quad_vertices_texCoords, sizeof(quad_vertices_texCoords));
        mirror_quadCMDs.add_attribute_floats_array(0, 2, 4, 0);
        mirror_quadCMDs.add_attribute_floats_array(1, 2, 4, 2);

        // load textures
        texture cubeTexture("extra/mariaKids.jpg");//wooden_container.jpg");
        texture floorTexture("metal.png");

        // shader configuration
        shader.use();
        shader.setInt("texture1", 0);

        screenShader.use();
        screenShader.setInt("texture1", 0);

        // framebuffer configuration
        frame_buffer fbo;
        fbo.as_renderbuffer(ogl_context.screen_width(), ogl_context.screen_height());
        frame_buffer::unbind();

        glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
        FlyCam camera(glm::vec3(0.f, 0.f, 3.f));

        while (!ogl_context.should_close()) {
            // bind to framebuffer and draw scene as we normally would to color texture
                fbo.bind();
                // make sure we clear the framebuffer's content
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
                glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

                shader.use();
                glm::mat4 model = glm::mat4(1.0f);
                glm::mat4 view = camera.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_context.screen_width() / (float)ogl_context.screen_height(), 0.1f, 100.0f);
                shader.setMat4("view", view);
                shader.setMat4("projection", projection);

                // cubes
                cubeCMDs.bind();
                cubeTexture.activate(GL_TEXTURE0);
                model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                // floor
                planeCMDs.bind();
                floorTexture.bind();
                shader.setMat4("model", glm::mat4(1.0f));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                attributes_binding_object::unbind();

                // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
                frame_buffer::unbind();
                // clear all relevant buffers
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyway)
                glClear(GL_COLOR_BUFFER_BIT);

                screenShader.use();
                mirror_quadCMDs.bind();
                glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
                // use the color attachment texture as the texture of the quad plane
                fbo.bind_texture();
                glDrawArrays(GL_TRIANGLES, 0, 6);

                ogl_context.swap();
                ogl_context.check_keys(camera);
        }
        return EXIT_SUCCESS;
}
