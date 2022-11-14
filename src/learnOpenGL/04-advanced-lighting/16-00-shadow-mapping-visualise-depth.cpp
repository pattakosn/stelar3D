#include "ogl_context.h"
#include "shader.h"
#include "datapoints.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "texture.h"
#include "depth_map_artifacts.h"

int main()
{
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader depth("../shaders/16-00-depth.vert", "../shaders/16-00-depth.frag");
    Shader depthDbg("../shaders/16-00-depth-dbg.vert", "../shaders/16-00-depth-dbg.frag");

    // plane VAO
    attributes_binding_object planeCMDs;
    planeCMDs.bind();
    vertex_array planeDAT;
    planeDAT.add_buffer(plane_vertices_shadows, sizeof(plane_vertices_shadows));
    planeCMDs.add_attribute_floats_array(0, 3, 8, 0);
    planeCMDs.add_attribute_floats_array(1, 3, 8, 3);
    planeCMDs.add_attribute_floats_array(2, 2, 8, 6);
    attributes_binding_object::unbind();

    // setup plane VAO
    attributes_binding_object quadCMD;
    quadCMD.bind();
    vertex_array quadDAT;
    quadDAT.add_buffer(quadVertices,sizeof(quadVertices));
    quadCMD.add_attribute_floats_array(0, 3, 5, 0);
    quadCMD.add_attribute_floats_array(1, 2, 5, 3);

    attributes_binding_object cubeCMD;
    cubeCMD.bind();
    vertex_array cubeDAT;
    cubeDAT.add_buffer(cube_vertices, sizeof(cube_vertices));
    cubeCMD.add_attribute_floats_array(0, 3, 8, 0);
    cubeCMD.add_attribute_floats_array(1, 3, 8, 3);
    cubeCMD.add_attribute_floats_array(2, 2, 8, 6);


    texture floor("../assets/wood.png"); // WOOD TEXTURE

    // configure depth map FBO
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    depth_map_artifacts depthMap(SHADOW_WIDTH, SHADOW_HEIGHT);

    // shader configuration
    depthDbg.use();
    depthDbg.setInt("depthMap", 0);

    // lighting info
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));
    bool quit = false;
    planeCMDs.bind();
    bool check;
    while (!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        depth.use();
        depth.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        depthMap.bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        floor.activate(GL_TEXTURE0);
        // floor
        glm::mat4 model = glm::mat4(1.0f);
        depth.setMat4("model", model);
        planeCMDs.bind();//glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // cubes
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        depth.setMat4("model", model);
        //renderCube();
        // render Cube
        cubeCMD.bind();//glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        attributes_binding_object::unbind();//glBindVertexArray(0);
        // model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.5f));
        depth.setMat4("model", model);
        //renderCube();
        // render Cube
        cubeCMD.bind();//glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        attributes_binding_object::unbind();//glBindVertexArray(0);
        // model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.25));
        depth.setMat4("model", model);
        //renderCube();
        // render Cube
        cubeCMD.bind();//glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        attributes_binding_object::unbind();//glBindVertexArray(0);
        frame_buffer::unbind();//glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, ogl_app.screen_width(), ogl_app.screen_height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render Depth map to quad for visual debugging
        // renders a 1x1 XY quad in NDC
        depthDbg.use();
        depthDbg.setFloat("near_plane", near_plane);
        depthDbg.setFloat("far_plane", far_plane);
        depthMap.activate_texture(GL_TEXTURE0);
        //renderQuad();
        quadCMD.bind();//glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        quadCMD.unbind();//glBindVertexArray(0);

        ogl_app.swap();
        handle_events(quit, my_cam, ogl_app, check);
    }
    return EXIT_SUCCESS;
}
