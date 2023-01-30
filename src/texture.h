#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/gl.h>
#include <string>
#include <vector>

class texture {
public:
    //std::string type; //NIKOS TODO how should i handle these?
    //std::string path;
    texture();
    explicit texture(std::string fname, bool flipped = false, bool gammaCorrection = false);
    explicit texture(std::vector<std::string>& faces);

    texture(const texture &) = delete;
    texture& operator=(const texture &) = delete;

    texture(texture&& other) noexcept;
    texture& operator=(texture&& other);

    ~texture();

    void bind();

    // text_num ie GL_TEXTURE0, ...
    void activate (int text_num);
    void set_cube_map();

    // wrapping: GL_REPEAT (default wrapping method), GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    void wrap_parameters (GLint s_wrap = GL_REPEAT, GLint t_wrap = GL_REPEAT) const;
    // filtering: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
    void filtering_parameters (GLint min_filter = GL_LINEAR, GLint mag_filter = GL_LINEAR) const;
    void load_image(const std::string& fname, bool flipped = false, bool gammaCorrection = false,
                    int s_wrap = GL_REPEAT, int t_wrap = GL_REPEAT, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
    GLuint get() { return tex; };
private:
    GLuint tex = 0UL;
    GLenum target = 0; // for now it should be: GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP. in the future i do not know
};
#endif
