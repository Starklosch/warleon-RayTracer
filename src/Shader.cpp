#include "Shader.h"

#include <fstream>

bool Shader::Compile()
{
    glCompileShader(id);

    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (!status) {
        GLint length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string message;
        message.resize(length);
        glGetShaderInfoLog(id, length, nullptr, message.data());

        std::cout << message;
        return false;
    }

    return true;
}

Shader::Shader(GLenum type, const std::string& path)
{
    this->type = type;
    id = glCreateShader(type);

    std::ifstream file(path);
    file.seekg(0, std::ios_base::end);
    int length = file.tellg();
    file.seekg(0);

    std::string code;
    code.resize(length);
    file.read(code.data(), length);

    const GLchar* source = code.c_str();
    glShaderSource(id, 1, &source, &length);
    
    Compile();
}

Shader::Shader(GLuint id)
{
    this->id = id;
}

Shader::~Shader()
{
    glDeleteShader(id);
}

GLenum Shader::GetType() const {
    return type;
}

Shader::operator GLuint() const {
    return id;
}

std::shared_ptr<Shader> Shader::Create(GLenum type, const std::string& path)
{
    return std::make_shared<Shader>(type, path);
}
