#include "ogl_context.h"
#include "shader.h"
#include "model.h"
#include "fly_cam.h"
#include "stb_image.h"
#include "win_cam_pos_fps.h"

int main(int, char*[]) {
    ogl_context ogl_app;
    glEnable(GL_DEPTH_TEST);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    constexpr auto textures_are_flipped = true;
    stbi_set_flip_vertically_on_load(textures_are_flipped);
    Shader sh_asteroid("13-15-asteroids.vert", "13-15-asteroids.frag");
    Shader sh_planet("13-15-planet.vert", "13-15-planet.frag");
    model rock("../assets/rock/rock.obj");
    model planet("../assets/planet/planet.obj");
    FlyCam my_cam(glm::vec3(0.f, 0.f, 55.f));
    //my_cam.MovementSpeed = 100.;

    // generate a large list of semi-random model transformation matrices
    constexpr size_t amount = 80000;
    std::vector<glm::mat4> modelMatrices;
    modelMatrices.reserve(amount);
    srand(static_cast<unsigned int>(ogl_app.time())); // initialize random seed
    float radius = 150.0;
    float offset = 25.0f;
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

    // configure instanced array
    vertex_array buffer;
    buffer.add_buffer(glm::value_ptr(modelMatrices[0]), amount * sizeof(glm::mat4));

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    rock.set_instanced_glm4data_every_mesh(3);

    win_cam_pos_fps_init((int)ogl_app.io->DisplaySize.x, (int)ogl_app.io->DisplaySize.y);

    while (!ogl_app.should_close()) {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)ogl_app.screen_width() / (float)ogl_app.screen_height(), 0.1f, 1000.0f);
        glm::mat4 view = my_cam.GetViewMatrix();

        sh_asteroid.use();
        sh_asteroid.setMat4("projection", projection);
        sh_asteroid.setMat4("view", view);
        sh_planet.use();
        sh_planet.setMat4("projection", projection);
        sh_planet.setMat4("view", view);

        // draw planet
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        sh_planet.setMat4("model", model);
        planet.draw(sh_planet);

        // draw meteorites
        sh_asteroid.use();
        sh_asteroid.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        rock.draw_instanced(amount);


        win_cam_pos_fps(my_cam, ogl_app);
        ogl_app.swap();
        FlyCam camera;
        ogl_app.check_keys(camera);
    }
    return EXIT_SUCCESS;
}
