#ifndef APP_H
#define APP_H

#include "Image.h"
#include "Button.h"

struct App {
    int windowWidth;
    int windowHeight;
    bool drawing = false;
    bool buttonPressed = false;
    bool buttonHighlighted = false;
    csci3081::Image* backgroundImage = nullptr;
    csci3081::Image* buttonImage = nullptr;
    csci3081::Button* button = nullptr;
};

#endif
