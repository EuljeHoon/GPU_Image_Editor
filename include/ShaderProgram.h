#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <glad/glad.h>

namespace csci3081 {
    class ShaderProgram {
        public:
            ShaderProgram();
            ~ShaderProgram();
            ShaderProgram(const ShaderProgram& other);
            ShaderProgram& operator=(const ShaderProgram& other);

            void runProgram();
            void uniform1i(const char* name, int value);
            void uniform3f(const char* name, float w, float h, float d);

            unsigned int getProgram() const;

        private:
            unsigned int program;
    };
}
#endif