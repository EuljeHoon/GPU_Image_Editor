#ifndef TEXTURERECTANGLE_H
#define TEXTURERECTANGLE_H

#include <glad/glad.h>

namespace csci3081 {
    class TexturedRectangle {
        public:
            // TexturedRectangle is always making a same default rectangle and adjusting with the shader program
            // so no need to implement default constructor and destructor
            TexturedRectangle();
            ~TexturedRectangle();
            
            TexturedRectangle(const TexturedRectangle& other);
            TexturedRectangle& operator=(const TexturedRectangle& other);
            //  Copy constructor and assignment operator were not implemented 
            // on main.cpp for TexturedRectangle
            void draw();
            
        private:
            unsigned int VAO;
            unsigned int VBO;
            unsigned int EBO;
    };
}
#endif