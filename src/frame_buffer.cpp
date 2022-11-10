#include <iostream>
#include "frame_buffer.h"

frame_buffer::frame_buffer() {
        glGenFramebuffers(1, &fbo);
}

frame_buffer::~frame_buffer() {
        glDeleteFramebuffers(1, &fbo);
}

void frame_buffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void frame_buffer::bind_texture(GLenum target) {
        glBindTexture(target, tb.get());
}

void frame_buffer::as_renderbuffer(int x, int y) {
        bind();

        // create a color attachment texture
        tb.bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (float)x, (float)y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        tb.filtering_parameters();    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tb.get(), 0);

        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        rbo.parameters(x, y);

        // now actually attach it
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo.get());

        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
                abort();
        }
}

void frame_buffer::as_depth_map(const unsigned int width, const unsigned int height) {
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

void frame_buffer::as_depth_map_fixed(const unsigned int width, const unsigned int height) {
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

void frame_buffer::activate_texture(GLenum id) {
    tb.activate(id);
}
void frame_buffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}