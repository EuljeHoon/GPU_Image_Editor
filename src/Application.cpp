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
    imageData = nullptr;
    waveData = nullptr;
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
    delete waveTexture;
    delete imageTexture;
    delete resetButton;
    delete colorButton;
    delete imageData;
    delete waveData;
    delete buttonImage;
}
void Application::runApplication() {
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

        // Quit on ESC
        if(window->getKey(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window->newWindow(), true);
        }
        // Toggle CPU/GPU coloring (for performance comparison)
        if(window->getKey(GLFW_KEY_G)) useCPU = false;
        if(window->getKey(GLFW_KEY_C)) useCPU = true;

        // Advance the wave one step on the CPU, then upload the result to the GPU texture
        waveData->waveStep(useCPU);

        float width = waveData->getWidth();
        float height = waveData->getHeight();
        waveTexture->copyData(waveData->getData(), width, height, 4);

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->runProgram();

        // Draw the wave texture across the whole screen (shader applies the colormap)
        glActiveTexture(GL_TEXTURE0);
        // imageTexture->bind();
        glBindTexture(GL_TEXTURE_2D, imageTexture->getTexture());
        shader->uniform1i("imageTex", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, waveTexture->getTexture());
        shader->uniform1i("waveTex", 1);

        shader->uniform3f("scale", 1.0f, 1.0f, 1.0f);
        shader->uniform3f("offset", 0.0f, 0.0f, 0.0f);
        shader->uniform1i("highlight", false);
        shader->uniform1i("cpuMode", false);
        texRectangle->draw();

        window->swapBuffers();
        window->pollEvents();
    }
}
void Application::initialImage() {
    imageData = new Image("img_small.jpeg");
    waveData = new Image("img_small.jpeg");
    waveData->waveInit();
    buttonImage = new Image("reset.png");
    colorButtonImage = new Image("Color_Button.png");
}
void Application::initialWindow() {
    float width = imageData->getWidth();
    float height = imageData->getHeight();
    window = new Window(width, height);
    glfwSetWindowUserPointer(window->newWindow(), this);
    window->viewPort(width, height);
    windowWidth = width;
    windowHeight = height;
}
void Application::initialShaderTexture() {
    float width = imageData->getWidth();
    float height = imageData->getHeight();
    shader = new ShaderProgram();
    texRectangle = new TexturedRectangle();
    imageTexture = new Texture(imageData->getData(), width, height, 4);
    waveTexture = new Texture(waveData->getData(), width, height, 4);
}
void Application::initialButton() {
    float width = imageData->getWidth();
    float height = imageData->getHeight();
    float aspect = 1.0f * width / height;
    resetButton = new Button(buttonImage, 0.01f, 0.01f, 0.1f, 0.1f * aspect);
}
void Application::initialColorButton() {
    float width = imageData->getWidth();
    float height = imageData->getHeight();
    float aspect = 1.0f * width / height;
    colorButton = new ColorButton(colorButtonImage, 0.01f, 0.17f, 0.1f, 0.1f * aspect);
}
