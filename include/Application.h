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
            unsigned int waveTex[2] = {0, 0};   // 두 장: 현재/다음 번갈아 (float, 높이 저장)
            unsigned int waveFbo[2] = {0, 0};   // 각 텍스처에 그릴 때 쓰는 FBO
            int currentIdx = 0;                 // 지금 "현재"가 몇 번인지 (0 or 1)
            void initWaveTextures();            // 텍스처 2장 + FBO 2개 만들기 (한 번)

            void initialImage();
            void initialWindow();
            void initialShaderTexture();
            void initialButton();
            void initialColorButton();
    };
}
#endif