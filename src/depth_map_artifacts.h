#ifndef STELAR3D_DEPTH_MAP_ARTIFACTS_H
#define STELAR3D_DEPTH_MAP_ARTIFACTS_H
#include "frame_buffer.h"

class depth_map_artifacts : public frame_buffer {
public:
    depth_map_artifacts(const unsigned int width, const unsigned int height);

    void activate_texture(GLenum id = GL_TEXTURE0);

    depth_map_artifacts(const depth_map_artifacts &) = delete;
    depth_map_artifacts& operator=(const depth_map_artifacts &) = delete;
    depth_map_artifacts(depth_map_artifacts&& other) = delete;
    depth_map_artifacts& operator=(depth_map_artifacts&& other) = delete;
    ~depth_map_artifacts() = default;
private:
//    frame_buffer dm;
};
#endif
