#include "Image.h"
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace csci3081;

// Memory management
Image::Image() {
    img = nullptr;
    width = 0;
    height = 0;
    channels = 0;
}

Image::Image(const std::string& fileName) {
    img = nullptr;
    load_image(fileName);
}

// Image Big Three
// If you have dynamic memory in a class, be sure to implement a copy constructor, the assignment operator, and a destructor.
Image::~Image() {
    if (img) {
        stbi_image_free(img);
        img = nullptr;
    }
}

Image::Image(const Image& other) {
    img = nullptr;
    width = 0;
    height = 0;
    channels = 0;
    fileName = "";
    *this = other;
}

Image& Image::operator=(const Image& other) {
    if (this == &other) {
        return *this;
    }
    if (img != nullptr) {
        stbi_image_free(img);
        img = nullptr;
    }
    if (other.img != nullptr) {
        int size = other.width * other.height * other.channels;
        img = new unsigned char[size];
        for (int i = 0; i < size; i++) {
            img[i] = other.img[i];
        }
        width = other.width;
        height = other.height;
        channels = other.channels;
        fileName = other.fileName;
    } else {
        width = 0;
        height = 0;
        channels = 0;
        fileName = "";
    }
    return *this;
}

bool Image::load_image(const std::string& fileName) {
    //Checking previous data exists
    if (img) {
        stbi_image_free(img);
        img = nullptr;
    }

    img = stbi_load(fileName.c_str(), &width, &height, &channels, 4);
    channels = 4;
    if (img == NULL) {
        std::cout << "Error in loading the image" << std::endl;
        exit(1);
    }
    this->fileName = fileName;
    std::cout << "Loaded image with a width of " << width << "px, a height of " << height << "px, and " << channels << " channels" << std::endl;
    return true;
}
unsigned char* Image::getData() const {
    return img;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

int Image::getChannels() const {
    return channels;
}
void Image::reset() {
    if (!fileName.empty()) {
        load_image(fileName);
    }
}

void Image::drawPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char a) {
    if (!img || x < 0 || y < 0 || x >= width || y >= height) {
        return;
    }
    int index = (y * width + x) * 4;
    img[index + 0] = red;
    img[index + 1] = green;
    img[index + 2] = blue;
    img[index + 3] = a;
}

void Image::fillPattern() {
    if (!img) return;
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            unsigned char value = (unsigned char) (255.0 * x / width);
            drawPixel(x, y, value, value, value, 255);
        }
    }
}

void Image::fillAnimated(float t) {
    if (!img) return;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float v = 0.5f + 0.5f * sinf((x * 0.05f) + t);
            unsigned char value = (unsigned char) (255.0f * v);
            drawPixel(x, y, value, value, value, 255);
        }
    }
}