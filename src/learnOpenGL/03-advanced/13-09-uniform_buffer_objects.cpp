#include "ogl_context.h"
#include "fly_cam.h"
#include "shader.h"
#include "datapoints.h"
#include "handle_events.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "win_cam_pos_fps.h"

int main(int, char*[])
{
        ogl_context my_context;
        glEnable(GL_DEPTH_TEST);

        Shader shaderRed("../shaders/uniform_buffer.vert", "../shaders/red.frag");
        Shader shaderGreen("../shaders/uniform_buffer.vert", "../shaders/green.frag");
        Shader shaderBlue("../shaders/uniform_buffer.vert", "../shaders/blue.frag");
        Shader shaderYellow("../shaders/uniform_buffer.vert", "../shaders/yellow.frag");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        attributes_binding_object cubeCMDs;
        cubeCMDs.bind();
        vertex_array cubeDAT;
        cubeDAT.add_buffer(cube_normals, sizeof(cube_normals));
        cubeCMDs.add_attribute_floats_array(0, 3, 6, 0);
        cubeCMDs.bind();

        // configure a uniform buffer object
        // first. We get the relevant block indices
        unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
        unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
        unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
        unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");
        // then we link each shader's uniform block to this uniform binding point
        glUniformBlockBinding(shaderRed.ID, uniformBlockIndexRed, 0);
        glUniformBlockBinding(shaderGreen.ID, uniformBlockIndexGreen, 0);
        glUniformBlockBinding(shaderBlue.ID, uniformBlockIndexBlue, 0);
        glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);
        // Now actually create the buffer
        unsigned int uboMatrices;
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        // define the range of the buffer that links to a uniform binding point
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

        // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
        glm::mat4 projection = glm::perspective(45.0f, (float)my_context.screen_width() / (float)my_context.screen_height(), 0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));
        win_cam_pos_fps_init((int)my_context.io->DisplaySize.x, (int)my_context.io->DisplaySize.y);

        bool quit = false;
        while (!quit) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
                glm::mat4 view = my_cam.GetViewMatrix();
                glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
                glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
                glBindBuffer(GL_UNIFORM_BUFFER, 0);

                // draw 4 cubes
                // RED
                cubeCMDs.bind();
                shaderRed.use();
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
                shaderRed.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                // GREEN
                shaderGreen.use();
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
                shaderGreen.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                // YELLOW
                shaderYellow.use();
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
                shaderYellow.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                // BLUE
                shaderBlue.use();
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
                shaderBlue.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                win_cam_pos_fps(my_cam, my_context);

                my_context.swap();
                handle_events(quit, my_cam, my_context);
        }
        return 0;
}
