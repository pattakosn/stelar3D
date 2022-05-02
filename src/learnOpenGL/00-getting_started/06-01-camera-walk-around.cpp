#include <iostream>
#include "shader.h"
#include "ogl_context.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "texture.h"
#include "datapoints.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main(int, char *[]) {
    ogl_context my_context;
    glEnable(GL_DEPTH_TEST);

    Shader my_shader("../shaders/05-00-coordinates.vert", "../shaders/05-00-coordinates.frag");

    vertex_array triangle_data;
    triangle_data.add_buffer(cube_texture_coords, sizeof(cube_texture_coords));

    attributes_binding_object triangle;
    triangle.bind();
    // position attribute
    triangle.add_attribute_floats_array(0, 3, 5, 0);
    // texture coord attribute
    triangle.add_attribute_floats_array(1, 2, 5, 3);

    texture wood_box("../assets/wooden_container.jpg", true);
    texture face("../assets/awesomeface.png", true);

    my_shader.use();

    glUniform1i(glGetUniformLocation(my_shader.ID, "texture0"), 0); // set it manually
    my_shader.setInt("texture1", 1); // or with shader class
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        my_shader.use();
        wood_box.activate(GL_TEXTURE0);
        face.activate(GL_TEXTURE1);
        glm::mat4 projection = glm::mat4(1.0f);
        // note that we're translating the scene in the reverse direction of where we want to move
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int viewLoc = glGetUniformLocation(my_shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        my_shader.setMat4("projection", projection);
        triangle.bind();
        for (unsigned int i = 0; i < 20; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_random_positions[i]);
            float angle = 20.0f * (i + 1);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, my_context.time() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            my_shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        my_context.swap();

        while (SDL_PollEvent(&event)) {
            float cameraSpeed = 2.5f * my_context.dt(); // adjust accordingly
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_w:
                        case SDLK_UP:
                            cameraPos += cameraSpeed * cameraFront;
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            cameraPos -= cameraSpeed * cameraFront;
                            break;
                        case SDLK_a:
                        case SDLK_LEFT:
                            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                            break;
                    }
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            std::cout << "MESSAGE:Resizing window...\n";
                            //resizeWindow(m_event.window.data1, m_event.window.data2);
                            std::cout << "MESSAGE: Window [width x height] = " << event.window.data1 << " x "
                                      << event.window.data2 << "\n";//%d, %d\n", windowWidth, windowHeight);
                            //m_camera->resizeWindow(windowWidth, windowHeight);
                            //TODO: move this to gl_context so as to update WIDTH/HEIGHT
                            glViewport(0, 0, event.window.data1, event.window.data2);
                            break;
                    }
                    break;
            }
        }
    }
    return EXIT_SUCCESS;
}