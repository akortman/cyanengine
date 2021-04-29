#include "shader.hpp"

#include "cyan/src/util/string.hpp"
#include "cyan/src/logging/error.hpp"
#include "cyan/src/logging/logger.hpp"

using namespace cyan::io::backends::opengl;

Shader::Shader() : shader_program(0) {}


Shader::Shader(Shader&& other) noexcept
{
    this->shader_program = other.shader_program;
    other.shader_program = 0;
}


Shader::Shader(const std::string& vert_path, const std::string& frag_path)
{
    auto vert_shader_src = string_util::file_contents(vert_path);
    auto frag_shader_src = string_util::file_contents(frag_path);

    auto vert = compile_shader(ShaderType::Vertex, vert_shader_src);
    auto frag = compile_shader(ShaderType::Fragment, frag_shader_src);

    this->shader_program = link_shaders(vert, frag);
    LOG(DEBUG, "Compiled & linked shader program {} from [vert] {} [frag] {}", shader_program, vert_path, frag_path);

    glDeleteShader(vert);
    glDeleteShader(frag);
}


void Shader::use()
{
    glUseProgram(shader_program);
}


Shader::~Shader()
{
    if (shader_program != 0) glDeleteProgram(shader_program);
}


Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (this->shader_program != 0) glDeleteProgram(this->shader_program);
        this->shader_program = other.shader_program;
        other.shader_program = 0;
    }
    return *this;
}


unsigned int cyan::io::backends::opengl::compile_shader(
        ShaderType shader_type,
        const std::string& shader_src)
{
    unsigned int shader = glCreateShader(GLenum(shader_type));
    const char *c_str = shader_src.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    // Check that the shader compiled successfully, and throw and error if it did not.
    int success;
    const int buf_size = 1024;
    char buf[buf_size];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::string shader_type_str = "Unknown";
        if (shader_type == ShaderType::Vertex) {
            shader_type_str = "Vertex";
        } else if (shader_type == ShaderType::Fragment) {
            shader_type_str = "Fragment";
        }
        glGetShaderInfoLog(shader, buf_size, nullptr, buf);
        throw cyan::Error("{} shader failed to link: {}", shader_type_str, buf);
    }

    return shader;
}


unsigned int cyan::io::backends::opengl::link_shaders(unsigned int vertex_shader, unsigned int fragment_shader)
{
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // Check that the program linked successfully, and throw and error if it did not.
    int success;
    const int buf_size = 1024;
    char buf[buf_size];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, buf_size, nullptr, buf);
        throw cyan::Error("Shader program failed to link: {}", buf);
    }

    return shader_program;
}

