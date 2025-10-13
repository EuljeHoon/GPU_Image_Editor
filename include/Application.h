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

            void initialImage();
            void initialWindow();
            void initialShaderTexture();
            void initialButton();
            void initialColorButton();
    };
}
#endif