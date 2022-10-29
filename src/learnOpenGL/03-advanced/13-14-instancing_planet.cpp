#include "ogl_context.h"
#include "shader.h"
#include "model.h"
#include "fly_cam.h"
#include "handle_events.h"
#include "stb_image.h"
#include "win_cam_pos_fps.h"

int main(int, char*[]) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    constexpr auto textures_are_flipped = true;
    stbi_set_flip_vertically_on_load(textures_are_flipped);
    Shader shader("../shaders/13-14-instancing_planet.vert", "../shaders/13-14-instancing_planet.frag");
    model rock("../assets/rock/rock.obj");
    model planet("../assets/planet/planet.obj");
    FlyCam my_cam(glm::vec3(0.f, 0.f, 55.f));
    //my_cam.MovementSpeed = 100.;

    // generate a large list of semi-random model transformation matrices
    const unsigned int amount = 80000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(ogl_app.time())); // initialize random seed
    float radius = 150.0;
    float offset = 25.f;
    for (unsigned int i = 0; i < amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    win_cam_pos_fps_init((int)ogl_app.io->DisplaySize.x, (int)ogl_app.io->DisplaySize.y);

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
                                                1000.0f);
        glm::mat4 view = my_cam.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.setMat4("model", model);
        planet.draw(shader);

        // draw meteorites
        for (unsigned int i = 0; i < amount; i++) {
            shader.setMat4("model", modelMatrices[i]);
            rock.draw(shader);
        }

        win_cam_pos_fps(my_cam, ogl_app);
        ogl_app.swap();
        handle_events(quit, my_cam, ogl_app);
    }
    delete modelMatrices;
    return EXIT_SUCCESS;
}
