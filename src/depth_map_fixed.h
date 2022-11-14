#ifndef STELAR3D_DEPTH_MAP_FIXED_H
#define STELAR3D_DEPTH_MAP_FIXED_H
#include "frame_buffer.h"
class depth_map_fixed : public frame_buffer {
public:
    depth_map_fixed(const unsigned int width, const unsigned int height);

    void activate_texture(GLenum id = GL_TEXTURE0);

    depth_map_fixed(const depth_map_fixed &) = delete;
    depth_map_fixed& operator=(const depth_map_fixed &) = delete;
    depth_map_fixed(depth_map_fixed&& other) = delete;
    depth_map_fixed& operator=(depth_map_fixed&& other) = delete;
    ~depth_map_fixed() = default;
private:
//    frame_buffer dm;
};
#endif
