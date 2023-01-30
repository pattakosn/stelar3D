#ifndef ATTRIBUTES_BINDING_OBJECT
#define ATTRIBUTES_BINDING_OBJECT
#include "glad/gl.h"
#include "GLFW/glfw3.h"

class attributes_binding_object {
public:
    attributes_binding_object();

    attributes_binding_object(const attributes_binding_object &) = delete;
    attributes_binding_object& operator=(const attributes_binding_object &) = delete;

    attributes_binding_object(attributes_binding_object&& other) noexcept;
    attributes_binding_object& operator=(attributes_binding_object&& other) noexcept;

    ~attributes_binding_object();

    void add_attribute(GLuint att, GLint elements, GLsizei stride, GLint offset);
    void add_attribute_floats_array(GLuint att, GLint elements, GLsizei stride, GLint offset);

    void bind() const;
    static void unbind();
    unsigned int id() { return vao;};
private:
    unsigned int vao = 0;
    void reset();
};
#endif
