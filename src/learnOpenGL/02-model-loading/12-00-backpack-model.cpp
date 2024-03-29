#include "ogl_context.h"
#include "shader.h"
#include "model.h"
#include "fly_cam.h"
#include "stb_image.h"

int main(int, char*[]) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    constexpr auto textures_are_flipped = true;

    stbi_set_flip_vertically_on_load(true);
    Shader shader("model.vert", "model.frag");

    model celt("../assets/extra/celt/Celt 07.fbx", textures_are_flipped);
    model backpack("../assets/backpack/backpack.obj", textures_are_flipped);
    model nanosuit("../assets/extra/nanosuit/nanosuit.obj", textures_are_flipped);
    model cyborg("../assets/extra/cyborg/cyborg.obj", textures_are_flipped);
    model planet("../assets/extra/planet/planet.obj", textures_are_flipped);
    model rock("../assets/extra/rock/rock.obj", textures_are_flipped);
    model vampire("../assets/extra/vampire/dancing_vampire.dae", textures_are_flipped);
    //model backpack("../assets/hk433/hk443.obj", textures_are_flipped);
    //model backpack("../assets/train/train.fbx", textures_are_flipped);

    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));
    //camera.MovementSpeed = 100.;

    while (!ogl_app.should_close()) {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                                (float) ogl_app.screen_width() / (float) ogl_app.screen_height(),
                                                0.1f,
                                                100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down, so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        backpack.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.5f, 4.5f, 0.0f));
        shader.setMat4("model", model);
        rock.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.f, 4.5f, 0.0f));
        shader.setMat4("model", model);
        nanosuit.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.f, 4.5f, 0.0f));
        shader.setMat4("model", model);
        cyborg.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.f, -4.5f, 0.0f));
        shader.setMat4("model", model);
        planet.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
        model = glm::translate(model, glm::vec3(30.f, 30.f, 0.0f));
        shader.setMat4("model", model);
        vampire.draw(shader);

        ogl_app.swap();
        ogl_app.check_keys(camera);
    }
    return EXIT_SUCCESS;
}
