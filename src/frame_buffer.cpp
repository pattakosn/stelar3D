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
void frame_buffer::initialize(int x, int y) {
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

void frame_buffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}