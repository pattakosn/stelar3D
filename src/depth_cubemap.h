#ifndef STELAR3D_DEPTH_CUBEMAP_H
#define STELAR3D_DEPTH_CUBEMAP_H
#include "frame_buffer.h"

class depth_cubemap : public frame_buffer {
public:
    depth_cubemap(const unsigned int width, const unsigned int height);

    depth_cubemap();

    void activate_texture(GLenum id = GL_TEXTURE0);

    depth_cubemap(const depth_cubemap &) = delete;
    depth_cubemap& operator=(const depth_cubemap &) = delete;
    depth_cubemap(depth_cubemap&& other) = delete;
    depth_cubemap& operator=(depth_cubemap&& other) = delete;
    ~depth_cubemap() = default;
};
#endif
