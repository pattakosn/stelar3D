#include "ogl_context.h"
#include "fly_cam.h"
#include "shader.h"
#include "datapoints.h"
#include "handle_events.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "texture.h"

int main(int, char*[])
{
        ogl_context my_context;//"sth", 480, 320);
        glEnable(GL_DEPTH_TEST);

        // build and compile shaders
        Shader shader("../shaders/P-MVP-1T.vert", "../shaders/1T-apply.frag");
        Shader screenShader("../shaders/P-1T.vert", "../shaders/1T-apply.frag");

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
        mirrorQuadDAT.add_buffer(quad_vertices, sizeof(quad_vertices));
        mirror_quadCMDs.add_attribute_floats_array(0, 2, 4, 0);
        mirror_quadCMDs.add_attribute_floats_array(1, 2, 4, 2);

        // load textures
        texture cubeTexture("../assets/wooden_container.jpg");
        texture floorTexture("../assets/metal.png");

        // shader configuration
        shader.use();
        shader.setInt("texture1", 0);

        screenShader.use();
        screenShader.setInt("texture1", 0);

        // framebuffer configuration
        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // create a color attachment texture
        unsigned int textureColorbuffer;
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (float)my_context.screen_width(), (float)my_context.screen_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (float)my_context.screen_width(), (float)my_context.screen_height()); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
                abort();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
        FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));

        bool quit = false;
        while(!quit) {
                // bind to framebuffer and draw scene as we normally would to color texture
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                // make sure we clear the framebuffer's content
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
                glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

                shader.use();
                glm::mat4 model = glm::mat4(1.0f);
                glm::mat4 view = my_cam.GetViewMatrix();
                glm::mat4 projection = glm::perspective(glm::radians(my_cam.Zoom), (float)my_context.screen_width() / (float)my_context.screen_height(), 0.1f, 100.0f);
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
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                // clear all relevant buffers
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
                glClear(GL_COLOR_BUFFER_BIT);

                screenShader.use();
                mirror_quadCMDs.bind();
                glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
                // use the color attachment texture as the texture of the quad plane
                glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                my_context.swap();
                handle_events(quit, my_cam, my_context);
        }
        glDeleteFramebuffers(1, &framebuffer);
        return EXIT_SUCCESS;
}
