#include "ShaderProgram.h"
#include <filesystem>

std::string_view intToName(GLint type) {
    switch (type) {
    case GL_BOOL:
        return ": bool";
    case GL_INT:
        return ": int";
    case GL_FLOAT:
        return ": float";

        // Vectors
    case GL_FLOAT_VEC2:
        return ": vec2";
    case GL_FLOAT_VEC3:
        return ": vec3";
    case GL_FLOAT_VEC4:
        return ": vec4";

        // Matrices
    case GL_FLOAT_MAT2:
        return ": mat2";
    case GL_FLOAT_MAT3:
        return ": mat3";
    case GL_FLOAT_MAT4:
        return ": mat4";
    }
    return ": unknown";
}

void ShaderProgram::PrintResource(Properties& props, std::string_view name) {

    std::cout << props.location;
    std::cout << intToName(props.type);
    std::cout << ' ' << name;
    if (props.size > 1)
        std::cout << '[' << props.size << "]\n";
    else
        std::cout << '\n';
}

ShaderProgram::ShaderProgram()
{
    id = 0;
}

ShaderProgram::ShaderProgram(const std::string& name)
{
	id = glCreateProgram();
    std::string vs = name + ".vert";
    std::string fs = name + ".frag";
    std::string gs = name + ".geom";

    Attach(GL_VERTEX_SHADER, vs);
    Attach(GL_FRAGMENT_SHADER, fs);
    Attach(GL_GEOMETRY_SHADER, gs);
    Link();
}

ShaderProgram::ShaderProgram(const ShaderProgram& program)
{
    id = glCreateProgram();

    Attach(program.vertShader);
    Attach(program.fragShader);
    Attach(program.geomShader);
    Attach(program.compShader);
    Link();
}

ShaderProgram::ShaderProgram(ShaderProgram&& program) noexcept
{
    id = program.id;
    program.id = 0;

    vertShader = program.vertShader;
    fragShader = program.fragShader;
    geomShader = program.geomShader;
    compShader = program.compShader;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(id);
}

void ShaderProgram::Attach(GLenum type, std::string path)
{
    if (!std::filesystem::exists(path))
            return;

    auto shader = Shader::Create(type, path);

    switch (type)
    {
    case GL_VERTEX_SHADER:
        if (vertShader)
            return;
        vertShader = shader;
        break;
    case GL_FRAGMENT_SHADER:
        if (fragShader)
            return;
        fragShader = shader;
        break;
    case GL_GEOMETRY_SHADER:
        if (geomShader)
            return;
        geomShader = shader;
        break;
    }

    glAttachShader(id, *shader);
}

void ShaderProgram::Attach(std::shared_ptr<Shader> shader)
{
    if (shader == nullptr)
        return;

    switch (shader->GetType())
    {
        case GL_VERTEX_SHADER:
            if (vertShader)
                return;
            vertShader = shader;
            break;
        case GL_FRAGMENT_SHADER:
            if (fragShader)
                return;
            fragShader = shader;
            break;
        case GL_GEOMETRY_SHADER:
            if (geomShader)
                return;
            geomShader = shader;
    }

	glAttachShader(id, *shader);
}

//void ShaderProgram::Attach(Shader&& shader)
//{
//    std::cout << "Temp shader id: " << shader << "\n";
//
//    switch (shader.GetType())
//    {
//    case GL_VERTEX_SHADER:
//        if (vertShader)
//            return;
//        vertShader = &shader;
//        break;
//    case GL_FRAGMENT_SHADER:
//        if (fragShader)
//            return;
//        fragShader = &shader;
//        break;
//    }
//
//    glAttachShader(id, shader);
//}

void ShaderProgram::Detach(std::shared_ptr<Shader> shader)
{
    glDetachShader(id, *shader);
}

bool ShaderProgram::Link()
{
	glLinkProgram(id);

    GLint status = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &status);

    if (!status) {
        GLint length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string message;
        message.resize(length);
        glGetProgramInfoLog(id, length, nullptr, message.data());

        std::cout << message;
        return false;
    }

    return true;
}

void ShaderProgram::Use()
{
	glUseProgram(id);
}

void ShaderProgram::SetUBO(const std::string& name, int bindingPoint)
{
    GLuint blockIndex = glGetUniformBlockIndex(id, name.c_str());
    glUniformBlockBinding(id, blockIndex, bindingPoint);
}

void ShaderProgram::Uniform(const std::string& name, GLint value) {
    GLint location = glGetUniformLocation(id, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::Uniform(const std::string& name, GLfloat value) {
    GLint location = glGetUniformLocation(id, name.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::Uniform(const std::string& name, GLdouble value) {
    GLint location = glGetUniformLocation(id, name.c_str());
    glUniform1d(location, value);
}

void ShaderProgram::Print() {
    GLint numActiveAttribs = 0;
    GLint numActiveUniforms = 0;
    //glGetProgramInterfaceiv(id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
    //glGetProgramInterfaceiv(id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    Properties actualProps;
    auto ptr = reinterpret_cast<GLint*>(&actualProps);

    GLenum props[] = { GL_TYPE, GL_LOCATION, GL_ARRAY_SIZE, GL_NAME_LENGTH };
    GLint propCount = sizeof(props) / sizeof(GLenum);

    std::cout << "\nShader " << id << ":\n\n";

    for (GLint i = 0; i < numActiveAttribs; i++)
    {
        // glGetActiveAttrib
        glGetProgramResourceiv(id, GL_PROGRAM_INPUT, i, propCount, props, propCount, nullptr, ptr);

        std::string name;
        name.resize(actualProps.length);
        glGetProgramResourceName(id, GL_PROGRAM_INPUT, i, actualProps.length, nullptr, name.data());

        std::cout << "Attribute #";
        PrintResource(actualProps, name);
    }

    for (GLint i = 0; i < numActiveUniforms; i++)
    {
        // glGetActiveUniform
        glGetProgramResourceiv(id, GL_UNIFORM, i, propCount, props, propCount, nullptr, ptr);

        std::string name;
        name.resize(actualProps.length);
        glGetProgramResourceName(id, GL_UNIFORM, i, actualProps.length, nullptr, name.data());

        std::cout << "Uniform #";
        PrintResource(actualProps, name);
    }
}

ShaderProgram::operator GLuint() const {
    return id;
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other)
{
    if (id != 0)
        glDeleteProgram(id);

    id = glCreateProgram();

    Attach(other.vertShader);
    Attach(other.fragShader);
    Attach(other.geomShader);
    Attach(other.compShader);
    Link();
    return *this;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    id = other.id;
    other.id = 0;

    vertShader = other.vertShader;
    fragShader = other.fragShader;
    geomShader = other.geomShader;
    compShader = other.compShader;
    return *this;
}
