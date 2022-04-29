#include "element_array.h"
#include <utility>

element_array::element_array() {
    glGenBuffers(1, &ebo);
}

void element_array::reset () {
    glDeleteBuffers(1, &ebo);
    ebo = 0;
}

element_array::~element_array() {
    reset();
}

element_array::element_array(element_array&& other) noexcept : ebo(other.ebo)  {
    other.ebo = 0;
}

element_array& element_array::operator=(element_array&& other) noexcept {
    if( this!= &other) {
        reset();
        std::swap(ebo, other.ebo);
    }
    return *this;
}

void element_array::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void element_array::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void element_array::add_buffer(unsigned int *data, std::size_t size, int draw_type) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, draw_type);
}
