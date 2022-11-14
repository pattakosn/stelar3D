#include "ogl_context.h"
#include "shader.h"
#include "datapoints.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "texture.h"
#include "depth_cubemap.h"

void renderCube(attributes_binding_object& cubeCMD) {
    cubeCMD.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    attributes_binding_object::unbind();
}

void renderScene(const Shader &shader, attributes_binding_object& cubeCMD) {
        // room cube
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(5.0f));
        shader.setMat4("model", model);
        glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
        shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
        renderCube(cubeCMD);
        shader.setInt("reverse_normals", 0); // and of course disable it
        glEnable(GL_CULL_FACE);
        // cubes
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        renderCube(cubeCMD);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.setMat4("model", model);
        renderCube(cubeCMD);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        renderCube(cubeCMD);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        renderCube(cubeCMD);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.75f));
        shader.setMat4("model", model);
        renderCube(cubeCMD);
    }

int main()
{
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // build and compile shaders
    Shader scene("../shaders/17-00-point-shadow.vert", "../shaders/17-00-point-shadow.frag");
    scene.use();
    scene.setInt("diffuseTexture", 0);
    scene.setInt("depthMap", 1);
    Shader depth("../shaders/17-00-point-shadow-depth.vert", "../shaders/17-00-point-shadow-depth.frag", "../shaders/17-00-point-shadow-depth.geom");

    // load textures
    texture floor("../assets/wood.png");

    attributes_binding_object cubeCMD;
    cubeCMD.bind();
    vertex_array cubeDAT;
    cubeDAT.add_buffer(cube_vertices, sizeof(cube_vertices));
    cubeCMD.add_attribute_floats_array(0, 3, 8, 0);
    cubeCMD.add_attribute_floats_array(1, 3, 8, 3);
    cubeCMD.add_attribute_floats_array(2, 2, 8, 6);

    // configure depth map FBO
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    depth_cubemap depthMap(SHADOW_WIDTH, SHADOW_HEIGHT);

    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
    FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));
    bool quit = false;
    bool shadows = true;
    while (!quit) {
        // move light position over time
        lightPos.z = static_cast<float>(sin(ogl_app.time() * 0.5) * 3.0);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 0. create depth cubemap transformation matrices
        float near_plane = 1.0f;
        float far_plane  = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

        // 1. render scene to depth cubemap
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        depthMap.bind();//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        depth.use();
        for (unsigned int i = 0; i < 6; ++i)
            depth.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        depth.setFloat("far_plane", far_plane);
        depth.setVec3("lightPos", lightPos);
        renderScene(depth, cubeCMD);
        depth_cubemap::unbind();//glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. render scene as normal
        glViewport(0, 0, ogl_app.screen_width(), ogl_app.screen_height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.use();
        glm::mat4 projection = glm::perspective(glm::radians(my_cam.Zoom), ((float)ogl_app.screen_width() / (float)ogl_app.screen_height()), 0.1f, 100.0f);
        glm::mat4 view = my_cam.GetViewMatrix();
        scene.setMat4("projection", projection);
        scene.setMat4("view", view);
        // set lighting uniforms
        scene.setVec3("lightPos", lightPos);
        scene.setVec3("viewPos", my_cam.Position);
        scene.setInt("shadows", shadows); // enable/disable shadows by pressing 'b'
        scene.setFloat("far_plane", far_plane);
        floor.activate(GL_TEXTURE0);
        depthMap.activate_texture(GL_TEXTURE1);
        renderScene(scene, cubeCMD);

        ogl_app.swap();
        handle_events(quit, my_cam, ogl_app, shadows);
    }
    return EXIT_SUCCESS;
}
