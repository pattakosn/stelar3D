#include "ogl_context.h"
#include "shader.h"
#include "model.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "stb_image.h"

int main(int, char*[]) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    constexpr auto textures_are_flipped = true;
    stbi_set_flip_vertically_on_load(textures_are_flipped);
    Shader shader("../shaders/13-12-visualize_normals-object.vert", "../shaders/13-12-visualize_normals-object.frag");
    Shader normalsShader("../shaders/13-12-visualize_normals-normals.vert", "../shaders/13-12-visualize_normals-normals.frag", "../shaders/13-12-visualize_normals-normals.geom");
    model backpack("../assets/backpack/backpack.obj", textures_are_flipped);
    FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));
    //my_cam.MovementSpeed = 100.;

    bool quit = false;
    while (!quit) {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shader.use();
        shader.setFloat("time", ogl_app.time());
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(my_cam.Zoom),
                                                (float) ogl_app.screen_width() / (float) ogl_app.screen_height(),
                                                0.1f,
                                                100.0f);
        glm::mat4 view = my_cam.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down, so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        backpack.draw(shader);

        normalsShader.use();
        normalsShader.setMat4("projection", projection);
        normalsShader.setMat4("view", view);
        normalsShader.setMat4("model", model);
        backpack.draw(normalsShader);

        ogl_app.swap();
        bool lol;
        handle_events(quit, my_cam, ogl_app, lol);
    }
    return EXIT_SUCCESS;
}
