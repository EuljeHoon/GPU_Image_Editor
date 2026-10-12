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

    initWaveTextures();   // ping-pong float textures + FBOs
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
        // Render Graphics
        // -------------------------------------

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->runProgram();

        // 현재 파동 텍스처를 화면에 띄우기
        // (아직 파동 계산이 없어서 전부 0 → 검은 화면이 정상)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waveTex[currentIdx]);
        shader->uniform1i("tex", 0);
        shader->uniform3f("scale", 1.0f, 1.0f, 1.0f);
        shader->uniform3f("offset", 0.0f, 0.0f, 0.0f);
        shader->uniform1i("highlight", false);
        shader->uniform1i("cpuMode", true);   // 지금은 텍스처 값 그대로 출력
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
// GPU WAVE — ping-pong float textures + FBOs
// ========================================================================
void Application::initWaveTextures() {
    int w = backgroundImage->getWidth();
    int h = backgroundImage->getHeight();

    for (int i = 0; i < 2; i++) {
        // 1) float texture (GL_R32F = 32-bit float, 1 channel → stores wave height)
        glGenTextures(1, &waveTex[i]);
        glBindTexture(GL_TEXTURE_2D, waveTex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // 2) FBO targeting this texture
        glGenFramebuffers(1, &waveFbo[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, waveFbo[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, waveTex[i], 0);

        // 3) Check status
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR: wave FBO " << i << " is not complete!" << std::endl;
        } else {
            std::cout << "wave FBO " << i << " ready." << std::endl;
        }

        // 4) Clear texture to 0 (flat, calm surface)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}