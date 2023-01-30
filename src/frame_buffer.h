#ifndef STELAR3D_FRAME_BUFFER_H
#define STELAR3D_FRAME_BUFFER_H
#include "glad/gl.h"
#include "texture.h"
#include "render_buffer.h"

class frame_buffer {
public:
        frame_buffer();
        frame_buffer(const frame_buffer &) = delete;
        frame_buffer& operator=(const frame_buffer &) = delete;
        frame_buffer(frame_buffer&& other) = delete;
        frame_buffer& operator=(frame_buffer&& other) = delete;
        ~frame_buffer();

        void bind();
        void bind_texture(GLenum target = GL_TEXTURE_2D);
        void as_renderbuffer(int x, int y);
        static void unbind();
protected:
        unsigned int fbo;
        texture tb;
        render_buffer rbo;
};
#endif
