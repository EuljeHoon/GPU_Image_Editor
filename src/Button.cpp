#include "Button.h"
#include <iostream>

using namespace csci3081;

Button::Button(Image* image, float xAxis, float yAxis, float w, float h) {
    buttonImage = image;
    x = xAxis;
    y = yAxis;
    width = w;
    height = h;
    highlighted = false;
    pressed = false;

    buttonTexture = new Texture();
    buttonTexture->load(image->getData(), image->getWidth(), image->getHeight(), 4);

}

// Button Big Three
Button::~Button() {
    delete buttonTexture;
}

Button::Button(const Button& other) {
    buttonImage = nullptr;
    buttonTexture = nullptr;
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    highlighted = false;
    pressed = false;
    *this = other;
}
Button& Button::operator=(const Button& other) {
    if (this == &other) {
        return *this;
    }
    delete buttonTexture;
    buttonTexture = nullptr;

    buttonImage = other.buttonImage;

    // Copy Button Texture
    if (other.buttonTexture != nullptr) {
        // Create new Texture
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
    return *this;
}

bool Button::buttonHighlighted(float cursorX, float cursorY) {
    if(cursorX >= x && cursorX <= x + width && cursorY >= y && cursorY <= y + height) {
        return true;
    }
    return false;
}
void Button::handlePress(Image* backgroundImage) {
    std::cout << "Clicked" << std::endl;
    pressed = true;
    if(backgroundImage) {
        backgroundImage->reset();
    }
}
void Button::resetPress() {
    pressed = false;
}
void Button::drawButton(ShaderProgram& shader, TexturedRectangle& texRectangle, bool buttonHighlighted) {
    buttonTexture->bind();
    shader.uniform1i("tex", 0);

    shader.uniform3f("scale", width, height, 1.0f);
    shader.uniform3f("offset", x * 2.0-1.0 + width, 1.0 - height - y * 2.0, 0.0f);
    shader.uniform1i("highlight", buttonHighlighted && !pressed);
    texRectangle.draw();
}
void Button::updateHeight(float aspect) {
    height = 0.1 * aspect;
}
float Button::getWidth() const {
    return width;
}
float Button::getHeight() const {
    return height;
}
float Button::getX() const {
    return x;
}
float Button::getY() const {
    return y;
}   
bool Button::getButtonHighlighted() const {
    return highlighted;
}
bool Button::getButtonPressed() const {
    return pressed;
}
