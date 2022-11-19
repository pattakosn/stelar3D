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
        Shader skyboxShader("../shaders/skybox.vert", "../shaders/skybox.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // cube VAO
        attributes_binding_object cubeCMDs;
        cubeCMDs.bind();
        vertex_array cubeDAT;
        cubeDAT.add_buffer(cube_texture_coords, sizeof(cube_texture_coords));
        cubeCMDs.add_attribute_floats_array(0, 3, 5, 0);
        cubeCMDs.add_attribute_floats_array(1, 2, 5, 3);
        // skybox VAO
        attributes_binding_object skyboxCMDs;
        skyboxCMDs.bind();
        vertex_array skyboxDAT;
        skyboxDAT.add_buffer(skyboxVertices, sizeof(skyboxVertices));
        skyboxCMDs.add_attribute_floats_array(0, 3, 3, 0);

        // load textures
        texture cubeTexture("../assets/wooden_container.jpg");
        std::vector<std::string> faces {
                "../assets/skybox/right.jpg",
                "../assets/skybox/left.jpg",
                "../assets/skybox/top.jpg",
                "../assets/skybox/bottom.jpg",
                "../assets/skybox/front.jpg",
                "../assets/skybox/back.jpg"
        };
        texture cubemapTexture(faces);

        // shader configuration
        shader.use();
        shader.setInt("texture1", 0);
        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);

        glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
        FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));

        bool quit = false;
        while(!quit) {
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

                // draw skybox as last
                // change depth function so depth test passes when values are equal to depth buffer's content
                glDepthFunc(GL_LEQUAL);
                skyboxShader.use();
                view = glm::mat4(glm::mat3(my_cam.GetViewMatrix())); // remove translation from the view matrix
                skyboxShader.setMat4("view", view);
                skyboxShader.setMat4("projection", projection);
                // skybox cube
                skyboxCMDs.bind();
                cubemapTexture.activate(GL_TEXTURE0);
                cubemapTexture.bind();
                glDrawArrays(GL_TRIANGLES, 0, 36);
                attributes_binding_object::unbind();
                glDepthFunc(GL_LESS); // set depth function back to default

                my_context.swap();
            bool lol;
            handle_events(quit, my_cam, my_context, lol);
        }
        return EXIT_SUCCESS;
}
