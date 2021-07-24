#include "renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "cyan/generated/components/Render.hpp"
#include "cyan/generated/components/Transform.hpp"

using namespace cyan::io::backends::opengl;

#define CHECK_GL_ERROR() check_gl_error(__FILE__, __LINE__)

const float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
};
const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};


Renderer::Renderer() {
    glewExperimental = true;
    if (!glfwInit()) {
        throw cyan::Error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    window = glfwCreateWindow(1024, 768, "cyanengine", nullptr, nullptr);
    if (window == nullptr){
        glfwTerminate();
        throw cyan::Error("Failed to open GLFW window");
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        throw cyan::Error("Failed to initialize GLEW");
    }

    CHECK_GL_ERROR();

    shader = Shader("../cyan/shader/vert.glsl", "../cyan/shader/frag.glsl");
    CHECK_GL_ERROR();
    shader.use();
    CHECK_GL_ERROR();

    // Generate vertex array and buffers.
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);  // Vertices
    glGenBuffers(1, &ebo);  // Indices

    glBindVertexArray(vao);

    // Add data to vertex buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    CHECK_GL_ERROR();

    // Add data to indices element buffer object.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    CHECK_GL_ERROR();

    // Set the vertex attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(0);
    CHECK_GL_ERROR();
}


void Renderer::draw(Scene& scene)
{
//    for (auto& entity: scene.ecs.iter_components<component::Render, component::Transform>()) {
//        // TODO: draw the entity.
//    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
}


const char* Renderer::name()
{
    return "OpenGL";
}


void Renderer::check_gl_error(const char* fname, int line) {
    auto err = glGetError();
    if (err == GL_NO_ERROR) return;

    std::string err_kind;
    switch (err) {
        case GL_INVALID_ENUM: err_kind = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE: err_kind = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: err_kind = "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW: err_kind = "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW: err_kind = "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY: err_kind = "GL_OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: err_kind = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        default: err_kind = "Unknown GL Error";
    }
    throw cyan::Error::make_error_with_location(fname, line, "[GL Error] {}", err_kind);
}
