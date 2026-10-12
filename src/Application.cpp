#include "Application.h"
#include <glad/glad.h>
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

    initFboTest();   // FBO test
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

        // --- FBO TEST: Draw Red Background ---
        renderToFbo();

        // -------------------------------------
        // Render Graphics
        // -------------------------------------

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->runProgram();

        // --- FBO TEST: Import Red background on the screen ---
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboTexture);
        shader->uniform1i("tex", 0);
        shader->uniform3f("scale", 1.0f, 1.0f, 1.0f);
        shader->uniform3f("offset", 0.0f, 0.0f, 0.0f);
        shader->uniform1i("highlight", false);
        shader->uniform1i("cpuMode", true);
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

// ========================================================================
// FBO TEST (temporary) — proves we can render into a texture, not the screen
// ========================================================================
void Application::initFboTest() {
    int w = backgroundImage->getWidth();
    int h = backgroundImage->getHeight();

    // 1) Create empty texture
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 2) Create FBO and assingn the texture above as a target
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, fboTexture, 0);

    // 3) Check FBO
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: FBO is not complete!" << std::endl;
    } else {
        std::cout << "FBO ready." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Application::renderToFbo() {
    int w = backgroundImage->getWidth();
    int h = backgroundImage->getHeight();

    // Turn on the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, w, h);

    // Red
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Turn off the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
}