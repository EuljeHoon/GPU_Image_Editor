#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

namespace csci3081 {
    class Texture {
        public:
            Texture();
            Texture(unsigned char* data, int width, int height, int channels);
            ~Texture();
            Texture(const Texture& other);
            Texture& operator=(const Texture& other);

            bool load(unsigned char* data, int width, int height, int channels);

            void copyData(unsigned char* data, int width, int height, int channels);

            void bind() const;
            unsigned int getTexture() const;
            int getWidth() const;
            int getHeight() const;
        
        private:
            unsigned int texture;
            int width;
            int height;
            int channels;
    };
}
#endif