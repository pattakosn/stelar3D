#pragma once
#include "glad/gl.h"

GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

class ogl_debug {
public:
    ogl_debug() {
        enable();
    }
    void enable(void);
};
