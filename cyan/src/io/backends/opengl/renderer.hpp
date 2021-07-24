#pragma once

#include "cyan/src/io/renderer_interface.hpp"
#include "shader.hpp"
#include "GLFW/glfw3.h"


namespace cyan::io::backends::opengl {
    struct Renderer: public RendererInterface {
        /// Create a Renderer.
        Renderer();

        /// Draw the contents of the ECS onto the screen (will render according to what Entities have Render components)
        void draw(Scene& scene) override;

        const char* name() override;

    private:
        /**
         * Helper function to check OpenGL for errors.
         * @param fname The file name to report, if an error is found.
         * @param line The error-causing line to report, if an error is found.
         */
        static void check_gl_error(const char* fname, int line);

        GLFWwindow* window;
        Shader shader;
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;
    };
}