#include "Application.h"
#include <iostream>

using namespace csci3081;

Application::Application(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    draw = false;
    buttonPressed = false;
    buttonHighlighted = false;
    colorButtonHighlighted = false;
    backgroundImage = nullptr;
    buttonImage = nullptr;
    resetButton = nullptr;
    colorButton = nullptr;

    initialImage();
    initialWindow();
    initialShaderTexture();
    initialButton();
    initialColorButton();
}
Application::~Application() {
    delete window;
    delete shader;
    delete texRectangle;
    delete backgroundTexture;
    delete resetButton;
    delete colorButton;
    delete backgroundImage;
    delete buttonImage;
}
void Application::runApplication() {
    // Scrapped from initial main.cpp
    while(!window->windowShouldClose())
    {
        // --- FPS calculation ---
        static double lastTime = glfwGetTime();
        static int frameCount = 0;
        frameCount++;
        double now = glfwGetTime();
        if (now - lastTime >= 1.0) {
            std::cout << (useCPU ? "[CPU] " : "[GPU] ")
                      << "FPS: " << frameCount
                      << "  (" << 1000.0 / frameCount << " ms/frame)" << std::endl;
            frameCount = 0;
            lastTime = now;
        }
        // -------------------------------------
        // Process window input (e.g. mouse movement, clicks, resize, etc...)
        // -------------------------------------
        // If the escape key is pressed, close the window
        if(window->getKey(GLFW_KEY_ESCAPE)) {
            //Set the window to close
            glfwSetWindowShouldClose(window->newWindow(), true);
        }
        if(window->getKey(GLFW_KEY_G)) useCPU = false;
        if(window->getKey(GLFW_KEY_C)) useCPU = true;

        // -------------------------------------
        // Copy background image data to the texture
        // -------------------------------------
        backgroundImage->waveStep(useCPU);
        
        float width = backgroundImage->getWidth();
        float height = backgroundImage->getHeight();
        backgroundTexture->copyData(backgroundImage->getData(), width, height, 4);

        // -------------------------------------
        // Render Graphics
        // -------------------------------------
        
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // -------------------------------------
        // Use the shader program defined above
        // -------------------------------------
        shader->runProgram();
        
        // -------------------------------------
        // Use the texture defined above
        // -------------------------------------
        backgroundTexture->bind();
        shader->uniform1i("tex", 0);
        shader->uniform3f("scale", 1.0f, 1.0f, 1.0f);
        shader->uniform3f("offset", 0.0f, 0.0f, 0.0f);
        shader->uniform1i("highlight", false);
        shader->uniform1i("cpuMode", useCPU);
        texRectangle->draw();

        // resetButton->drawButton(*shader, *texRectangle, buttonHighlighted);
        // colorButton->drawButton(*shader, *texRectangle, colorButtonHighlighted);

        // -------------------------------------
        // Show window on the screen
        // -------------------------------------
        window->swapBuffers();

        // -------------------------------------
        // Check for user input (mouse movement, clicks, keyboard, etc...)
        // -------------------------------------
        window->pollEvents();
    }
}
void Application::initialImage() {
    backgroundImage = new Image("img_small.jpeg");
    backgroundImage->waveInit();
    buttonImage = new Image("reset.png");
    colorButtonImage = new Image("Color_Button.png");
}
void Application::initialWindow() {
    float width = backgroundImage->getWidth();
    float height = backgroundImage->getHeight();
    window = new Window(width, height);
    glfwSetWindowUserPointer(window->newWindow(), this);
    window->viewPort(width, height);
    windowWidth = width;
    windowHeight = height;
}
void Application::initialShaderTexture() {
    float width = backgroundImage->getWidth();
    float height = backgroundImage->getHeight();
    shader = new ShaderProgram();
    texRectangle = new TexturedRectangle();
    backgroundTexture = new Texture(backgroundImage->getData(), width, height, 4);
}
void Application::initialButton() {
    float width = backgroundImage->getWidth();
    float height = backgroundImage->getHeight();
    float aspect = 1.0f * width / height;
    resetButton = new Button(buttonImage, 0.01f, 0.01f, 0.1f, 0.1f * aspect);
}
void Application::initialColorButton() {
    float width = backgroundImage->getWidth();
    float height = backgroundImage->getHeight();
    float aspect = 1.0f * width / height;
    colorButton = new ColorButton(colorButtonImage, 0.01f, 0.17f, 0.1f, 0.1f * aspect);
}