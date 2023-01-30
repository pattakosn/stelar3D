#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include "glad/gl.h"
#include <cstddef>

class vertex_array {
public:
    vertex_array();

    vertex_array(const vertex_array &) = delete;
    vertex_array& operator=(const vertex_array &) = delete;

    vertex_array(vertex_array&& other) noexcept;
    vertex_array& operator=(vertex_array&& other);

    ~vertex_array();

    // GL_DYNAMIC_DRAW, GL_STREAM_DRAW
    void add_buffer(float* data, std::size_t size, int draw_type = GL_STATIC_DRAW);
    void bind();
    static void unbind();
private:
    unsigned int vbo;
    void reset();
};
#endif
