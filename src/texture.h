#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include <string>

class texture {
public:
    //std::string type; //NIKOS TODO how should i handle these?
    //std::string path;
    texture();

    texture(const texture &) = delete;
    texture& operator=(const texture &) = delete;

    texture(texture&& other) noexcept;
    texture& operator=(texture&& other);

    explicit texture(std::string fname, bool flipped = false);
    ~texture();

    void bind();

    // text_num ie GL_TEXTURE0, ...
    void activate (int text_num);

    // wrapping: GL_REPEAT (default wrapping method), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    // filtering: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
    void wrap_parameters (int s_wrap = GL_REPEAT, int t_wrap = GL_REPEAT, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
    void load_image(const std::string& fname, bool flipped = false,
                    int s_wrap = GL_REPEAT, int t_wrap = GL_REPEAT, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
private:
    GLuint tex = 0UL;
};
#endif
