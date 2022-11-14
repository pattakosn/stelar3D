#include "depth_map_artifacts.h"

depth_map_artifacts::depth_map_artifacts(const unsigned int width, const unsigned int height) {
    tb.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    tb.filtering_parameters(GL_NEAREST, GL_NEAREST);
    tb.wrap_parameters();

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tb.get(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    unbind();
}
void depth_map_artifacts::activate_texture(GLenum id) {
    tb.activate(id);
}