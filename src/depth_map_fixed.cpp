#include "depth_map_fixed.h"

depth_map_fixed::depth_map_fixed(const unsigned int width, const unsigned int height) {
    tb.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    tb.filtering_parameters(GL_NEAREST, GL_NEAREST);
    tb.wrap_parameters(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tb.get(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    unbind();
}

void depth_map_fixed::activate_texture(GLenum id) {
    tb.activate(id);
}