#include "texture.h"
#include <iostream>
#include <cassert>
#include "stb_image.h"

texture::texture() {
    glGenTextures(1, &tex);
    target = GL_TEXTURE_2D; //TODO i do not like this here but did to quickly have a depth map framebuffer
}

texture::texture(const std::string fname, bool flipped, bool gammaCorrection) : texture() {
    target = GL_TEXTURE_2D;
    load_image(fname, flipped, gammaCorrection);
}

// loads a cubemap texture from 6 individual texture faces order:
// +X (right) / -X (left)
// +Y (top)   / -Y (bottom)
// +Z (front) / -Z (back)
texture::texture(std::vector<std::string>& faces) : texture() {
        target = GL_TEXTURE_CUBE_MAP;
        bind();

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++) {
                unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                if (data) {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                } else {
                        std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                }
                stbi_image_free(data);
        }
        // filtering_parameters(GL_TEXTURE_CUBE_MAP);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // wrap_parameters(GL_TEXTURE_CUBE_MAP);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

texture::texture(texture &&other) noexcept: tex(other.tex) { //, type(other.type), path(other.path) {
    other.tex = 0;
}

texture &texture::operator=(texture &&other) {
    if( this != &other ) {
        assert(1==0 && "finish me later");
    }
    return *this;
}

texture::~texture() {
    glDeleteTextures(1, &tex);
    tex = 0;
    glBindTexture(GL_TEXTURE_2D, 0); // better to now unbind current tex object
}

void texture::bind() {
    assert(tex && "texture id is zero");
    glBindTexture(target, tex);
    assert(glIsTexture(tex) && "This is not a texture..."); // attention: glIsTexture driver implementations may not conform to spec description, so we check AFTER binding
}

void texture::load_image(const std::string& fname, bool flipped, bool gammaCorrection,
                         int s_wrap, int t_wrap, int min_filter, int mag_filter) {
        bind();
        wrap_parameters(s_wrap, t_wrap);
        filtering_parameters(min_filter, mag_filter);

        stbi_set_flip_vertically_on_load(flipped);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        unsigned char *data = stbi_load(fname.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum internalFormat;
            GLenum dataFormat;
            if (nrChannels == 1) {
                internalFormat = dataFormat = GL_RED;
            } else if (nrChannels == 3) {
                internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
                dataFormat = GL_RGB;
            } else if (nrChannels == 4) {
                internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
                dataFormat = GL_RGBA;
            } else {
                std::cerr << "how is it possible to have " << nrChannels << " channels in " << fname.c_str() <<"?\n";
                std::abort();
            }
            std::cout << "Loading texture: " << fname.c_str() << " of size: " << width << "x" << height << " of " << nrChannels << std::endl;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture: " << fname.c_str() << std::endl;
        }
        stbi_image_free(data);
        assert(glIsTexture(tex) && "This is not a texture...");
}

void texture::wrap_parameters (int s_wrap, int t_wrap) const {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);
}

void texture::filtering_parameters (GLint min_filter, GLint mag_filter) const {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void texture::activate (int text_num) {
    glActiveTexture(text_num);
    bind();
}
