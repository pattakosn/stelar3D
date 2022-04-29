#include "texture.h"
#include <iostream>
#include <cassert>
#include "stb_image.h"

texture::texture() {
    glGenTextures(1, &tex);
}

texture::texture(const std::string fname, bool flipped) : texture() {
    load_image(fname);
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
    glBindTexture(GL_TEXTURE_2D, tex);
    assert(glIsTexture(tex) && "This is not a texture..."); // attention: glIsTexture driver implementations may not conform to spec description, so we check AFTER binding
}

void texture::load_image(const std::string& fname, bool flipped,
                         int s_wrap, int t_wrap, int min_filter, int mag_filter) {
    bind();
    wrap_parameters(s_wrap, t_wrap, min_filter, mag_filter);

    stbi_set_flip_vertically_on_load(flipped);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(fname.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "how is it possible to have " << nrChannels << " channels in " << fname.c_str() <<"?\n";
            std::abort();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << fname.c_str() << std::endl;
    }
    stbi_image_free(data);
    assert(glIsTexture(tex) && "This is not a texture...");
}

void texture::wrap_parameters (int s_wrap, int t_wrap, int min_filter, int mag_filter) {
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void texture::activate (int text_num) {
    glActiveTexture(text_num);
    bind();
}
