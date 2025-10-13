#include "ColorButton.h"
#include <iostream>

using namespace csci3081;

ColorButton::ColorButton(Image* image, float xAxis, float yAxis, float w, float h) {
    buttonImage = image;
    x = xAxis;
    y = yAxis;
    width = w;
    height = h;
    highlighted = false;
    pressed = false;
    currentColor = 0;
    
    // 2D array to store the values for palette. Start from blue, then red, then green
    // Blue
    colors[0][0] = 0;
    colors[0][1] = 0;
    colors[0][2] = 255;
    // Red
    colors[1][0] = 255;
    colors[1][1] = 0;
    colors[1][2] = 0;
    // Green
    colors[2][0] = 0;
    colors[2][1] = 255;
    colors[2][2] = 0;

    buttonTexture = new Texture();
    buttonTexture->load(image->getData(), image->getWidth(), image->getHeight(), 4);

}

// ColorButton Big Three
ColorButton::~ColorButton() {
    delete buttonTexture;
}
ColorButton::ColorButton(const ColorButton& other) {
    buttonImage = nullptr;
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    highlighted = false;
    pressed = false;
    currentColor = 0;
    buttonTexture = nullptr;
    *this = other;
}

ColorButton& ColorButton::operator=(const ColorButton& other) {
    if (this == &other) {
        return *this;
    }
    delete buttonTexture;
    buttonTexture = nullptr;
    buttonImage = other.buttonImage;

    // Copy ColorButton Texture
    if (other.buttonTexture != nullptr) {
        buttonTexture = new Texture();
        *buttonTexture = *other.buttonTexture;
    } else {
        buttonTexture = nullptr;
    }
    x = other.x;
    y = other.y;
    width = other.width;
    height = other.height;
    highlighted = other.highlighted;
    pressed = other.pressed;
    
    currentColor = other.currentColor;
    
    // copy colors array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            colors[i][j] = other.colors[i][j];
        }
    }
    
    return *this;
}

bool ColorButton::buttonHighlighted(float cursorX, float cursorY) {
    if(cursorX >= x && cursorX <= x + width && cursorY >= y && cursorY <= y + height) {
        return true;
    }
    return false;
}
// Color Index changes
void ColorButton::handlePress(Image* backgroundImage) {
    std::cout << "Clicked" << std::endl;
    currentColor++;
    if(currentColor > 2) {
        currentColor = 0;
    }
    pressed = true;
}
void ColorButton::resetPress() {
    pressed = false;
}
void ColorButton::drawButton(ShaderProgram& shader, TexturedRectangle& texRectangle, bool buttonHighlighted) {
    buttonTexture->bind();
    shader.uniform1i("tex", 0);

    shader.uniform3f("scale", width, height, 1.0f);
    shader.uniform3f("offset", x * 2.0-1.0 + width, 1.0 - height - y * 2.0, 0.0f);
    shader.uniform1i("highlight", buttonHighlighted && !pressed);
    texRectangle.draw();
}
void ColorButton::updateHeight(float aspect) {
    height = 0.1 * aspect;
}
float ColorButton::getWidth() const {
    return width;
}
float ColorButton::getHeight() const {
    return height;
}
float ColorButton::getX() const {
    return x;
}
float ColorButton::getY() const {
    return y;
}   
bool ColorButton::getButtonHighlighted() const {
    return highlighted;
}
bool ColorButton::getButtonPressed() const {
    return pressed;
}
// Get the color values for the current color
int ColorButton::getRed() const {
    return colors[currentColor][0];
}
int ColorButton::getGreen() const {
    return colors[currentColor][1];
}
int ColorButton::getBlue() const {
    return colors[currentColor][2];
}