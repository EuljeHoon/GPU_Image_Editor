#ifndef BUTTON_H
#define BUTTON_H

#include "Image.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "TexturedRectangle.h"

namespace csci3081 {
    class Button {
        public:
            // Button needs specific Image, location and size so no need to implement default constructor
            Button(Image* image, float xAxis, float yAxis, float w, float h);
            ~Button();
            Button(const Button& other);
            Button& operator=(const Button& other);

            bool buttonHighlighted(float cursorX, float cursorY);
            void handlePress(Image* backgroundImage);
            void resetPress();
            void drawButton(ShaderProgram& shader, TexturedRectangle& texRectangle, bool buttonHighlighted);
            void updateHeight(float aspect);
            
            float getWidth() const;
            float getHeight() const;
            float getX() const;
            float getY() const;
            bool getButtonHighlighted() const;
            bool getButtonPressed() const;
            
        private:
            Image* buttonImage;
            Texture* buttonTexture;
            float x;
            float y;
            float width;
            float height;
            bool highlighted;
            bool pressed;
    };
}
#endif