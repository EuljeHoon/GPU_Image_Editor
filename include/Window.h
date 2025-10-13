#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace csci3081 {
    class Window {
        public:
            Window(int width, int height);
            ~Window();
            Window(const Window& other);
            Window& operator=(const Window& other);

            bool getKey(int key) const;
            bool windowShouldClose() const;
            void swapBuffers();
            void pollEvents();
            void viewPort(int width, int height);

            GLFWwindow* newWindow() const;

            static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
            static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
            static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        
            private:
                GLFWwindow* window;
                int windowWidth;
                int windowHeight;
    };
}
#endif