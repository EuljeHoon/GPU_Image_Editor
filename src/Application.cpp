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
    initComputeShader();  // 파동 계산 셰이더
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

        // --- 테스트용: 스페이스 누르면 가운데를 "툭" 치기 ---
        if(window->getKey(GLFW_KEY_SPACE)) {
            clickX = 0.5f;
            clickY = 0.5f;
            clickStrength = 0.3f;
        }

        // --- GPU에서 파동 한 스텝 계산 ---
        waveStepGPU();

        // -------------------------------------
        // Render Graphics
        // -------------------------------------

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->runProgram();

        // 현재 파동 텍스처를 화면에 띄우기 (컬러맵)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waveTex[currIdx]);
        shader->uniform1i("tex", 0);
        shader->uniform3f("scale", 1.0f, 1.0f, 1.0f);
        shader->uniform3f("offset", 0.0f, 0.0f, 0.0f);
        shader->uniform1i("highlight", false);
        shader->uniform1i("cpuMode", false);   // 셰이더 컬러맵으로 색칠
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

    for (int i = 0; i < 3; i++) {
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

// ------------------------------------------------------------------------
// 파동 계산 셰이더 (GPU에서 파동 한 스텝을 계산)
// ------------------------------------------------------------------------
void Application::initComputeShader() {
    // 정점 셰이더: 전체 화면 사각형을 그대로 통과시키고 UV만 넘김
    const char* vs =
        "#version 330 core\n"
        "layout(location=0) in vec3 aPos;\n"
        "layout(location=1) in vec2 coord;\n"
        "out vec2 uv;\n"
        "void main(){\n"
        "   uv = coord;\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "}\0";

    // 프래그먼트 셰이더: 각 셀의 "다음 높이"를 계산
    //  - curr: 현재 격자 텍스처
    //  - prev: 이전 격자 텍스처
    //  - texel: 1픽셀의 UV 크기 (이웃 샘플링용)
    //  - c: 파동 속도
    const char* fs =
        "#version 330 core\n"
        "out float FragColor;\n"
        "in vec2 uv;\n"
        "uniform sampler2D curr;\n"
        "uniform sampler2D prev;\n"
        "uniform vec2 texel;\n"
        "uniform float c;\n"
        "uniform vec2 clickPos;\n"
        "uniform float clickStrength;\n"
        "void main(){\n"
        "   float center = texture(curr, uv).r;\n"
        "   float old    = texture(prev, uv).r;\n"
        "   float left   = texture(curr, uv + vec2(-texel.x, 0.0)).r;\n"
        "   float right  = texture(curr, uv + vec2( texel.x, 0.0)).r;\n"
        "   float up     = texture(curr, uv + vec2(0.0, -texel.y)).r;\n"
        "   float down   = texture(curr, uv + vec2(0.0,  texel.y)).r;\n"
        "\n"
        "   // DEBUG: 파동 공식 대신 그냥 복사만\n"
        "   float next = center;\n"
        "\n"
        "   // 클릭 위치 근처면 솟게 하기 (툭)\n"
        "   if (clickStrength > 0.0) {\n"
        "       float d = distance(uv, clickPos);\n"
        "       if (d < 0.05) { next = clickStrength; }\n"
        "   }\n"
        "   FragColor = next;\n"
        "}\0";

    // 컴파일 + 링크 (ShaderProgram.cpp 와 같은 방식)
    unsigned int v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vs, NULL); glCompileShader(v);
    int ok; char log[512];
    glGetShaderiv(v, GL_COMPILE_STATUS, &ok);
    if (!ok) { glGetShaderInfoLog(v, 512, NULL, log); std::cout << "COMPUTE VERTEX FAIL\n" << log << std::endl; }

    unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fs, NULL); glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &ok);
    if (!ok) { glGetShaderInfoLog(f, 512, NULL, log); std::cout << "COMPUTE FRAGMENT FAIL\n" << log << std::endl; }

    computeProgram = glCreateProgram();
    glAttachShader(computeProgram, v);
    glAttachShader(computeProgram, f);
    glLinkProgram(computeProgram);
    glGetProgramiv(computeProgram, GL_LINK_STATUS, &ok);
    if (!ok) { glGetProgramInfoLog(computeProgram, 512, NULL, log); std::cout << "COMPUTE LINK FAIL\n" << log << std::endl; }
    else { std::cout << "compute shader ready." << std::endl; }

    glDeleteShader(v);
    glDeleteShader(f);
}

int Application::computeLoc(const char* name) {
    return glGetUniformLocation(computeProgram, name);
}

// GPU에서 파동 한 스텝: curr/prev 읽어서 → next 텍스처에 씀 → 역할 교대
void Application::waveStepGPU() {
    int w = backgroundImage->getWidth();
    int h = backgroundImage->getHeight();

    // next 텍스처에 그리도록 FBO 바인딩
    glBindFramebuffer(GL_FRAMEBUFFER, waveFbo[nextIdx]);
    glViewport(0, 0, w, h);

    glUseProgram(computeProgram);

    // curr 텍스처 → 유닛 0, prev 텍스처 → 유닛 1
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waveTex[currIdx]);
    glUniform1i(computeLoc("curr"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, waveTex[prevIdx]);
    glUniform1i(computeLoc("prev"), 1);

    glUniform2f(computeLoc("texel"), 1.0f / w, 1.0f / h);  // 1픽셀 UV 크기
    glUniform1f(computeLoc("c"), 0.1f);
    glUniform2f(computeLoc("clickPos"), clickX, clickY);
    glUniform1f(computeLoc("clickStrength"), clickStrength);

    texRectangle->draw();   // 전체 화면 사각형 그림 → 셰이더가 모든 셀 계산

    clickStrength = 0.0f;   // 클릭은 한 프레임만 적용

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);

    // 역할 교대: 다음 프레임엔 "현재"가 "이전"이, "다음"이 "현재"가 됨
    int tmp = prevIdx;
    prevIdx = currIdx;
    currIdx = nextIdx;
    nextIdx = tmp;
}