#ifndef APPLICATION_H
#define APPLICATION_H

#include "Window.h"
#include "Image.h"
#include "Texture.h"
#include "TexturedRectangle.h"
#include "ShaderProgram.h"
#include "Button.h"
#include "ColorButton.h"

namespace csci3081 {
    class Application {
        public:
            int windowWidth;
            int windowHeight;
            bool draw;
            bool buttonPressed;
            bool buttonHighlighted;
            bool colorButtonHighlighted;
            bool useCPU = false;
            Image* backgroundImage;
            Image* buttonImage;
            Image* colorButtonImage;
            Button* resetButton;
            ColorButton* colorButton;
            Application(int width, int height);
            ~Application();
            void runApplication();
        private:
            Window* window;
            ShaderProgram* shader;
            TexturedRectangle* texRectangle;
            Texture* backgroundTexture;

            // --- GPU wave (ping-pong) ---
            unsigned int waveTex[3] = {0, 0, 0};  // 3장: 이전/현재/다음 (float, 높이 저장)
            unsigned int waveFbo[3] = {0, 0, 0};  // 각 텍스처에 그릴 때 쓰는 FBO
            int prevIdx = 0, currIdx = 1, nextIdx = 2;  // 역할 (매 프레임 돌려씀)
            unsigned int computeProgram = 0;      // 파동 계산 셰이더 프로그램
            float clickX = 0.0f, clickY = 0.0f;   // 클릭 위치 (UV 0~1)
            float clickStrength = 0.0f;           // 이번 프레임 클릭 세기 (0이면 클릭 없음)
            int computeLoc(const char* name);     // 계산 셰이더 uniform 위치 헬퍼
            void initWaveTextures();              // 텍스처 3장 + FBO 3개 만들기 (한 번)
            void initComputeShader();             // 파동 계산 셰이더 컴파일 (한 번)
            void waveStepGPU();                   // GPU에서 파동 한 스텝 (ping-pong)

            void initialImage();
            void initialWindow();
            void initialShaderTexture();
            void initialButton();
            void initialColorButton();
    };
}
#endif