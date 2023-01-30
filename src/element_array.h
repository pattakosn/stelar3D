#ifndef ELEMENT_ARRAY_H
#define ELEMENT_ARRAY_H
#include "glad/gl.h"
#include <cstddef>

class element_array {
public:
    element_array();
    
    element_array(const element_array &) = delete;
    element_array& operator=(const element_array &) = delete;

    element_array(element_array&& other) noexcept;
    element_array& operator=(element_array&& other) noexcept;
    
    ~element_array();

    // GL_DYNAMIC_DRAW, GL_STREAM_DRAW
    void add_buffer(unsigned int* data, std::size_t size, int draw_type = GL_STATIC_DRAW);
    void bind() const;
    static void unbind();
private:
    unsigned int ebo;
    void reset();
};
#endif
