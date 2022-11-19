#include "ogl_context.h"
#include "fly_cam.h"
#include "shader.h"
#include "datapoints.h"
#include "handle_events.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"

int main(int, char*[]) {
    ogl_context my_context;
    glEnable(GL_DEPTH_TEST);

    Shader houseGeomShader("../shaders/13-10-house_geometry.vert", "../shaders/13-10-house_geometry.frag",
                  "../shaders/13-10-house_geometry.geom");

    attributes_binding_object houseCMDs;
    houseCMDs.bind();
    vertex_array houseDAT;
    houseDAT.add_buffer(houseGeomPoints,sizeof(houseGeomPoints));
    houseCMDs.add_attribute_floats_array(0,2,5,0);
    houseCMDs.add_attribute_floats_array(1,3,5,2);
    attributes_binding_object::unbind();

    FlyCam my_cam(glm::vec3(0.f, 0.f, 3.f));

    bool quit = false;
    while(!quit) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        houseGeomShader.use();
        houseCMDs.bind();
        glDrawArrays(GL_POINTS, 0, 4);

        my_context.swap();
        bool lol;
        handle_events(quit, my_cam, my_context, lol);
    }
    return EXIT_SUCCESS;
}
