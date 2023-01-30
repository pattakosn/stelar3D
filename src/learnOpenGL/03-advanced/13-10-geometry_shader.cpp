#include "ogl_context.h"
#include "fly_cam.h"
#include "shader.h"
#include "datapoints.h"
#include "attributes_binding_object.h"
#include "vertex_array.h"

int main(int, char*[]) {
    ogl_context ogl_context;
    glEnable(GL_DEPTH_TEST);

    Shader houseGeomShader("13-10-house_geometry.vert", "13-10-house_geometry.frag",
                  "13-10-house_geometry.geom");

    attributes_binding_object houseCMDs;
    houseCMDs.bind();
    vertex_array houseDAT;
    houseDAT.add_buffer(houseGeomPoints,sizeof(houseGeomPoints));
    houseCMDs.add_attribute_floats_array(0,2,5,0);
    houseCMDs.add_attribute_floats_array(1,3,5,2);
    attributes_binding_object::unbind();

    FlyCam camera(glm::vec3(0.f, 0.f, 3.f));

    while (!ogl_context.should_close()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        houseGeomShader.use();
        houseCMDs.bind();
        glDrawArrays(GL_POINTS, 0, 4);

        ogl_context.swap();
        ogl_context.check_keys(camera);
    }
    return EXIT_SUCCESS;
}
