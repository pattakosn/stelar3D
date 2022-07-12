#include "render_buffer.h"

render_buffer::render_buffer() {
        glGenRenderbuffers(1, &rbo);
}

render_buffer::~render_buffer() {
        glDeleteRenderbuffers(1, &rbo);
}
void render_buffer::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

// by default use a single renderbuffer object for both a depth AND stencil buffer
void render_buffer::parameters(GLsizei x, GLsizei y, GLenum format) {
        bind();
        glRenderbufferStorage(GL_RENDERBUFFER, format, x, y);
}

GLuint render_buffer::get() {
        return rbo;
}