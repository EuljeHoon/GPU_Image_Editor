#include "Window.h"
#include "Application.h"
#include <iostream>
#include <GLFW/glfw3.h>  
#include <glad/glad.h>

using namespace csci3081;

Window::Window(int width, int height) {
    windowWidth = width;
    windowHeight = height;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(width, height, "Image Editor", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
}

// Window Big Three
Window::~Window() {
    glfwTerminate();
}
Window::Window(const Window& other) {
    window = nullptr;
    windowWidth = 0;
    windowHeight = 0;
    *this = other;
}
Window& Window::operator=(const Window& other) {
    if (this == &other) {
        return *this;
    }
    glfwTerminate();
    windowWidth = other.windowWidth;
    windowHeight = other.windowHeight;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(windowWidth, windowHeight, "Image Editor", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return *this;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return *this;
    }
    return *this;
}
bool Window::getKey(int key) const {
    return glfwGetKey(window, key) == GLFW_PRESS;
}
bool Window::windowShouldClose() const {
    return glfwWindowShouldClose(window);
}
void Window::swapBuffers() {
    glfwSwapBuffers(window);
}
void Window::pollEvents() {
    glfwPollEvents();
}
void Window::viewPort(int width, int height) {
    glViewport(0, 0, width, height);
}
GLFWwindow* Window::newWindow() const {
    return window;
}
void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // -------------------------------------
    // Get the applicaiton
    // -------------------------------------
    Application& app = *static_cast<Application*>(glfwGetWindowUserPointer(window));

    // -------------------------------------
    // If the mouse is over the button, highlight it
    // -------------------------------------
    float x = xpos/app.windowWidth;
    float y = ypos/app.windowHeight;
    app.buttonHighlighted = app.resetButton->buttonHighlighted(x, y);
    app.colorButtonHighlighted = app.colorButton->buttonHighlighted(x, y);

    // -------------------------------------
    // If we are drawing, edit the background image
    // -------------------------------------
    if (app.draw) {
        std::cout << x << " " << y << std::endl;
        int imgX = x * app.backgroundImage->getWidth();
        int imgY = y * app.backgroundImage->getHeight();
        int radius = 2;
        for (int i = imgX-radius; i < imgX+radius+1; i++) {
            for (int j = imgY-radius; j < imgY + radius+1; j++) {
                // Bring the color information from the color button
                int red = app.colorButton->getRed();
                int green = app.colorButton->getGreen();
                int blue = app.colorButton->getBlue();
                app.backgroundImage->drawPixel(i, j, red, green, blue, 255);
            }
        }
    }
}
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // -------------------------------------
    // Set the window drawing area
    // -------------------------------------
    glViewport(0, 0, width, height);

    
    // -------------------------------------
    // Get the applicaiton
    // -------------------------------------
    Application& app = *static_cast<Application*>(glfwGetWindowUserPointer(window));
    app.windowWidth = width;
    app.windowHeight = height;

    // -------------------------------------
    // update the button height
    // -------------------------------------
    float aspect = 1.0f*width/height;
    app.resetButton->updateHeight(aspect);
    app.colorButton->updateHeight(aspect);
}
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // -------------------------------------
    // handle button presses and drawing
    // -------------------------------------
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Application& app = *static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app.buttonHighlighted) {
            app.resetButton->handlePress(app.backgroundImage);
        }
        else if (app.colorButtonHighlighted) {
            app.colorButton->handlePress(app.backgroundImage);
        }
        else {
            app.draw = true;
        }
    }
    if (action == GLFW_RELEASE) {
        Application& app = *static_cast<Application*>(glfwGetWindowUserPointer(window));
        app.buttonPressed= false;
        app.draw = false;
        app.resetButton->resetPress();
        app.colorButton->resetPress();
    }
}