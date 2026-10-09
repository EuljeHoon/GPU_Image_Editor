#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <algorithm>

namespace csci3081 {
    class Image {
        public:
            Image();
            Image(const std::string& fileName);
            ~Image();
            Image(const Image& other);
            Image& operator=(const Image& other);

            bool load_image(const std::string& fileName);
            
            void fillPattern();
            void drawPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char a = 255);
            void reset();

            unsigned char* getData() const;
            int getWidth() const;
            int getHeight() const;
            int getChannels() const;
        private:
            unsigned char* img;
            int width;
            int height;
            int channels;
            std::string fileName;
    };
}
#endif