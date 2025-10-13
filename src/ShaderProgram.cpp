#include "ShaderProgram.h"
#include <iostream>

using namespace csci3081;

ShaderProgram::ShaderProgram() {
    program = 0;
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 coord;\n"
    "uniform vec3 offset;\n"
    "uniform vec3 scale;\n"
    "out vec2 interpCoord;\n"
    "void main()\n"
    "{\n"
    "   interpCoord = coord;\n"
    //"   float scale = 0.5;\n"
    "   gl_Position = vec4(aPos.xyz*scale + offset, 1.0);\n"
    "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D tex;\n"
    "uniform bool highlight;\n"
    "in vec2 interpCoord;\n"
    "void main()\n"
    "{\n"
    "   vec4 color = texture(tex, interpCoord); \n"
    "   if (color.a < 0.9) {discard;} \n"
    "   if (highlight) { \n"
    "       FragColor = clamp(color*vec4(2.0,2.0,2.0,1.0), 0, 1.0); \n"
    "   } \n"
    "   else { \n"
    "       FragColor = texture(tex, interpCoord);\n"
    "   } \n"
    "}\n\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// ShaderProgram Big Three
ShaderProgram::~ShaderProgram() {
    //May need condition for deleting the program
    glDeleteProgram(program);
}
ShaderProgram::ShaderProgram(const ShaderProgram& other) {
    program = 0;
    *this = other;
}
ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {
    if (this == &other) {
        return *this;
    }
    glDeleteProgram(program);
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 coord;\n"
    "uniform vec3 offset;\n"
    "uniform vec3 scale;\n"
    "out vec2 interpCoord;\n"
    "void main()\n"
    "{\n"
    "   interpCoord = coord;\n"
    //"   float scale = 0.5;\n"
    "   gl_Position = vec4(aPos.xyz*scale + offset, 1.0);\n"
    "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D tex;\n"
    "uniform bool highlight;\n"
    "in vec2 interpCoord;\n"
    "void main()\n"
    "{\n"
    "   vec4 color = texture(tex, interpCoord); \n"
    "   if (color.a < 0.9) {discard;} \n"
    "   if (highlight) { \n"
    "       FragColor = clamp(color*vec4(2.0,2.0,2.0,1.0), 0, 1.0); \n"
    "   } \n"
    "   else { \n"
    "       FragColor = texture(tex, interpCoord);\n"
    "   } \n"
    "}\n\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return *this;
}
void ShaderProgram::runProgram() {
    glUseProgram(program);
}
// glUniform1i calls from main.cpp
void ShaderProgram::uniform1i(const char* name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

// glUniform3f calls from main.cpp
void ShaderProgram::uniform3f(const char* name, float w, float h, float d) {
    glUniform3f(glGetUniformLocation(program, name), w, h, d);
}
unsigned int ShaderProgram::getProgram() const {
    return program;
}