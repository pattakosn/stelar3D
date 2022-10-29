#include "ogl_context.h"
#include "shader.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "datapoints.h"
#include "fly_cam.h"
#include "handle_events.h"

int main()
{
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    Shader shader("../shaders/13-13-instancing_glId.vert", "../shaders/13-13-instancing_glId.frag");

    // generate a list of 100 quad locations/translation-vectors
    glm::vec2 translations[100];
    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2) {
        for (int x = -10; x < 10; x += 2) {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    // store instance data in an array buffer
    vertex_array instanceDAT;
    instanceDAT.add_buffer(glm::value_ptr(translations[0]), sizeof(glm::vec2) * 100);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    attributes_binding_object quadCMDs;
    quadCMDs.bind();
    vertex_array quadDAT;
    quadDAT.add_buffer(quadVerticesColor, sizeof(quadVerticesColor));
    quadCMDs.add_attribute_floats_array(0, 2, 5, 0);
    quadCMDs.add_attribute_floats_array(1, 3, 5, 2);

    // also set instance data
    instanceDAT.bind(); // this attribute comes from a different vertex buffer
    quadCMDs.add_attribute_floats_array(2, 2, 2, 0);
    vertex_array::unbind();
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    glm::vec3 lightPos(0.75f, 0.75f, 0.75f);
    FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));

    bool quit = false;
    while(!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw 100 instanced quads
        shader.use();
        quadCMDs.bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
        glBindVertexArray(0);

        ogl_app.swap();
        handle_events(quit, my_cam, ogl_app);
    }
    return EXIT_SUCCESS;
}

