#ifndef COLOR_BUTTON_H
#define COLOR_BUTTON_H

#include "Image.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "TexturedRectangle.h"

//--------------------------------------------------------------------------------------------------
// ColorButton is a button that changes the color of the drawing pixel between blue, red, and green
//--------------------------------------------------------------------------------------------------
namespace csci3081 {
    class ColorButton {
        public:
            // Button needs specific Image, location and size so no need to implement default constructor
            ColorButton(Image* image, float xAxis, float yAxis, float w, float h);
            ~ColorButton();
            ColorButton(const ColorButton& other);
            ColorButton& operator=(const ColorButton& other);

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
            
            int getRed() const;
            int getGreen() const;
            int getBlue() const;
        private:
            Image* buttonImage;
            Texture* buttonTexture;
            float x;
            float y;
            float width;
            float height;
            bool highlighted;
            bool pressed;
            int currentColor;
            int colors[3][3];
    };
}
#endif