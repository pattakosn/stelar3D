#include "ogl_context.h"
#include "shader.h"
#include "texture.h"
#include "fly_cam.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"
#include "datapoints.h"

// renderQuad() renders a 1x1 XY quad in NDC
void renderQuad(attributes_binding_object& quadCMD) {
    quadCMD.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    attributes_binding_object::unbind();
}

// renderCube() renders a 1x1 3D cube in NDC.
void renderCube(attributes_binding_object& cubeCMD) {
    cubeCMD.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    attributes_binding_object::unbind();
}

int main(int, char*[]) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    Shader shader("20-00.lighting.vert", "20-00.lighting.frag");
    shader.use();
    shader.setInt("diffuseTexture", 0);
    Shader hdrShader("20-00.hdr.vert", "20-00.hdr.frag");
    hdrShader.use();
    hdrShader.setInt("hdrBuffer", 0);

    // note that we're loading the texture as an SRGB texture
    texture woodTexture("wood.png");

    // configure floating point framebuffer
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ogl_app.screen_width(), ogl_app.screen_height(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ogl_app.screen_width(), ogl_app.screen_height());
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    attributes_binding_object quadCMD;
    quadCMD.bind();
    vertex_array quadDAT;
    quadDAT.add_buffer(quadVertices, sizeof(quadVertices));
    quadCMD.add_attribute_floats_array(0, 3, 5, 0);
    quadCMD.add_attribute_floats_array(1, 2, 5, 3);

    attributes_binding_object cubeCMD;
    cubeCMD.bind();
    vertex_array cubeDAT;
    cubeDAT.add_buffer(cube_vertices, sizeof(cube_vertices));
    cubeCMD.add_attribute_floats_array(0, 3, 8, 0);
    cubeCMD.add_attribute_floats_array(1, 3, 8, 3);
    cubeCMD.add_attribute_floats_array(2, 2, 8, 6);

    // lighting info
    // positions
    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3( 0.0f,  0.0f, 49.5f)); // back light
    lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
    lightPositions.push_back(glm::vec3( 0.0f, -1.8f, 4.0f));
    lightPositions.push_back(glm::vec3( 0.8f, -1.7f, 6.0f));
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
    lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));
    bool hdr = true;
    float exposure = 20.0f;
    while (!ogl_app.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render scene into floating point framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        woodTexture.activate(GL_TEXTURE0);
        // set lighting uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++) {
            shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }
        shader.setVec3("viewPos", camera.Position);
        // render tunnel
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
        shader.setMat4("model", model);
        shader.setInt("inverse_normals", true);
        renderCube(cubeCMD);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        hdrShader.setInt("hdr", hdr);
        hdrShader.setFloat("exposure", exposure);
        renderQuad(quadCMD);

        ogl_app.swap();
        if ( glfwGetKey(ogl_app.get_win(), GLFW_KEY_H) == GLFW_PRESS)
            hdr = !hdr;
        std::cout << "hdr: " << (hdr ? "on" : "off") << " | exposure: " << exposure << std::endl;
        ogl_app.check_keys(camera);
    }
    return EXIT_SUCCESS;
}
