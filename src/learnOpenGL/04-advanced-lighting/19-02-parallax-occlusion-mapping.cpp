#include "ogl_context.h"
#include "shader.h"
#include "vertex_array.h"
#include "attributes_binding_object.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "texture.h"

// renders a 1x1 quad in NDC with manually calculated tangent vectors
void renderQuad(attributes_binding_object& quadCMD) {
    quadCMD.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    attributes_binding_object::unbind();
}

int main() {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    Shader shader("19-02-parallax-occlusion-mapping.vert", "19-02-parallax-occlusion-mapping.frag");
    shader.use();
    shader.setInt("diffuseMap", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("depthMap", 2);

//    texture diffuseMap("bricks2.jpg");
//    texture normalMap("bricks2_normal.jpg");
//    texture heightMap("bricks2_disp.jpg");
    texture diffuseMap("wood.png");
    texture normalMap("wood_normal.png");
    texture heightMap("wood_disp.png");

    attributes_binding_object quadCMD;
    quadCMD.bind();
    vertex_array quadDAT;
    {
        // positions
        glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
        glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        float quadVertices[] = {
                // positions            // normal         // texcoords  // tangent                          // bitangent
                pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
                pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

                pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
                pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        quadDAT.add_buffer(quadVertices, sizeof(quadVertices));
        quadCMD.add_attribute_floats_array(0, 3, 14, 0);
        quadCMD.add_attribute_floats_array(1, 3, 14, 3);
        quadCMD.add_attribute_floats_array(2, 2, 14, 6);
        quadCMD.add_attribute_floats_array(3, 3, 14, 8);
        quadCMD.add_attribute_floats_array(4, 3, 14, 11);
    }

    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));
    float heightScale = 0.1f;
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//        if (heightScale > 0.0f)
//            heightScale -= 0.0005f;
//        else
//            heightScale = 0.0f;
//    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
//        if (heightScale < 1.0f)
//            heightScale += 0.0005f;
//        else
//            heightScale = 1.0f;
//    }
    bool quit = false;
    bool shadows = true;
    while (!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // configure view/projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // render parallax-mapped quad
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(ogl_app.time() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show parallax mapping from multiple directions
        shader.setMat4("model", model);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setFloat("heightScale", heightScale); // adjust with Q and E keys
        std::cout << heightScale << std::endl;
        diffuseMap.activate(GL_TEXTURE0);
        normalMap.activate(GL_TEXTURE1);
        heightMap.activate(GL_TEXTURE2);
        renderQuad(quadCMD);

        // render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        shader.setMat4("model", model);
        renderQuad(quadCMD );

        ogl_app.swap();
        handle_events(quit, camera, ogl_app, shadows);
    }
    return EXIT_SUCCESS;
}
