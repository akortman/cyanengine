#include "renderer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


using namespace cyan::draw2d::backends::opengl;


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

    GLFWwindow* window;
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
}


void Renderer::draw(cyan::ECS& ecs)
{
    for (auto& entity: ecs.iter_components<component::Render, component::Transform>()) {
        // TODO: draw the entity.
    }
}
