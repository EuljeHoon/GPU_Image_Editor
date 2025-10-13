#include "Texture.h"
#include <iostream>

using namespace csci3081;

Texture::Texture() {
    texture = 0;
    width = 0;
    height = 0;
    channels = 0;
    glGenTextures(1, &texture);
}

Texture::Texture(unsigned char* data, int width, int height, int channels) {
    texture = 0;
    width = 0;
    height = 0;
    channels = 0;
    glGenTextures(1, &texture);
    load(data, width, height, channels);
}

// Texture Big Three
Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

Texture::Texture(const Texture& other) {
    texture = 0;
    width = 0;
    height = 0;
    channels = 0;
    *this = other;
}

Texture& Texture::operator=(const Texture& other) {
    if (this == &other) {
        return *this;
    }
    if (texture != 0) {
        glDeleteTextures(1, &texture);
    }
    glGenTextures(1, &texture);
    if(other.texture != 0) {
        width = other.width;
        height = other.height;
        channels = other.channels;
        glBindTexture(GL_TEXTURE_2D, texture);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        width = 0;
        height = 0;
        channels = 0;
        texture = 0;
    }
    return *this;
}

bool Texture::load(unsigned char* data, int width, int height, int channels) {
    this->width = width;
    this->height = height;
    this->channels = channels;

    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return true;
}

void Texture::copyData(unsigned char* data, int width, int height, int channels) {
    this->width = width;
    this->height = height;
    this->channels = channels;

    glBindTexture(GL_TEXTURE_2D, texture);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned int Texture::getTexture() const {
    return texture;
}

int Texture::getWidth() const {
    return width;
}

int Texture::getHeight() const {
    return height;
}