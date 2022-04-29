#include "attributes_binding_object.h"
#include <utility>

attributes_binding_object::attributes_binding_object() {
    glGenVertexArrays(1, &vao);
}

void attributes_binding_object::reset () {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
}

attributes_binding_object::~attributes_binding_object() {
    reset();
}

attributes_binding_object::attributes_binding_object(attributes_binding_object&& other) noexcept: vao(other.vao) {
    other.vao = 0;
}

attributes_binding_object& attributes_binding_object::operator=(attributes_binding_object&& other) noexcept {
    if( this!= &other) {
        reset();
        std::swap(vao, other.vao);
    }
    return *this;
}

void attributes_binding_object::bind() const {
    glBindVertexArray(vao);
}

void attributes_binding_object::unbind() {
    glBindVertexArray(0);
}


//NIKOS TODO: can you better encaptulate this in respect to ELEMENT_TYPE/stride/etc
void attributes_binding_object::add_attribute(GLuint att, GLint elements, GLsizei stride, GLint offset) {
    glEnableVertexAttribArray(att);
    glVertexAttribPointer(att, elements, GL_FLOAT, GL_FALSE, stride, (void*)offset);
}

void attributes_binding_object::add_attribute_floats_array(GLuint att, GLint elements, GLsizei stride, GLint offset) {
    glEnableVertexAttribArray(att);
    glVertexAttribPointer(att, elements, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
}
