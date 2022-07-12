#ifndef STELAR3D_RENDER_BUFFER_H
#define STELAR3D_RENDER_BUFFER_H
#include "glad/glad.h"

class render_buffer {
public:
        render_buffer();
        render_buffer(const render_buffer &) = delete;
        render_buffer& operator=(const render_buffer &) = delete;
        render_buffer(render_buffer&& other) = delete;
        render_buffer& operator=(render_buffer&& other) = delete;
        ~render_buffer();

        void bind();
        // by default use a single renderbuffer object for both a depth AND stencil buffer
        void parameters(GLsizei x, GLsizei y, GLenum format = GL_DEPTH24_STENCIL8);
        GLuint get();
private:
        GLuint rbo;
};
#endif
