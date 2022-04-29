#include "vertex_array.h"
#include <utility>

vertex_array::vertex_array() {
    glGenBuffers(1, &vbo);
}

void vertex_array::reset () {
    glDeleteBuffers(1, &vbo);
    vbo = 0;
}

vertex_array::vertex_array(vertex_array&& other) noexcept: vbo(other.vbo) {
    other.vbo = 0;
}

vertex_array& vertex_array::operator=(vertex_array&& other) {
    if( this!= &other) {
        reset();
        std::swap(vbo, other.vbo);
    }
    return *this;
}

vertex_array::~vertex_array() {
    reset();
}

void vertex_array::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void vertex_array::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_array::add_buffer(float* data, std::size_t size, int draw_type) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, draw_type);
}
